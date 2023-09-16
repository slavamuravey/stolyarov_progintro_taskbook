#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include "dynamic_array.h"
#include "server.h"
#include "connection.h"
#include "utils.h"

#define LISTEN_BACKLOG 16

ssize_t counter = 0;

struct connection_handler_data {
    struct server *srv;
    struct connection *con;
    pthread_mutex_t *mtx;
    sem_t *sem;
};

struct server *server_create(int domain, struct sockaddr *addr)
{
    int opt;
    struct server *srv;
    int s = socket(domain, SOCK_STREAM, 0);
    if (s == -1) {
        perror("socket");
        exit(1);
    }
    
    opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(s, (struct sockaddr *)addr, sizeof(*addr)) == -1) {
        perror("bind");
        close(s);
        exit(1);
    }

    if (listen(s, LISTEN_BACKLOG) == -1) {
        perror("listen");
        close(s);
        exit(1);
    }

    srv = malloc(sizeof(struct server));
    srv->socket = s;
    srv->connections = dynamic_array_create(2, sizeof(struct connection*));

    return srv;
}

static void server_print_info(struct server *srv)
{
    printf("Clients count: %ld\n", srv->connections->len);
}

static void connection_close(struct server *srv, struct connection *con)
{
    int i;
    for (i = 0; i < srv->connections->len; i++) {
        struct connection **cons = srv->connections->ptr;
        if (cons[i] == con) {
            dynamic_array_remove(srv->connections, i);
        }
    }
    
    shutdown(con->socket, SHUT_RDWR);
    close(con->socket);
    connection_destroy(con);
}

static void *connection_handler(void *data)
{
    struct connection_handler_data *ch_data = data;
    struct server *srv = ch_data->srv;
    struct connection *con = ch_data->con;
    pthread_mutex_t *mtx = ch_data->mtx;
    sem_t *sem = ch_data->sem;

    sem_post(sem);

    while (1) {
        char *buf = con->buf->ptr;
        ssize_t count;
        size_t nbytes = BUFSIZE - con->buf->len;
        char cmd[BUFSIZE];

        if (nbytes == 0) {
            char *response = "Input buffer oveflow\n";
            if (write(con->socket, response, strlen(response)) == -1) {
                perror("write");
                close(con->socket);
                close(srv->socket);
                exit(1);
            }
            pthread_mutex_lock(mtx);
            connection_close(srv, con);
            pthread_mutex_unlock(mtx);

            printf("Client disconnected\n");
            server_print_info(srv);

            return NULL;
        }

        count = read(con->socket, buf + con->buf->len, nbytes);
        if (count == -1) {
            perror("read");
            close(con->socket);
            close(srv->socket);
            exit(1);
        }

        if (count == 0) {
            pthread_mutex_lock(mtx);
            connection_close(srv, con);
            pthread_mutex_unlock(mtx);

            printf("Client disconnected\n");
            server_print_info(srv);

            return NULL;
        }

        con->buf->len += count;

        while (!extract_cmd(con->buf, cmd)) {
            char response[32];

            if (match_cmd(cmd, "up")) {
                pthread_mutex_lock(mtx);
                counter++;
                pthread_mutex_unlock(mtx);
                strcpy(response, "Ok\n");
            } else if (match_cmd(cmd, "down")) {
                pthread_mutex_lock(mtx);
                counter--;
                pthread_mutex_unlock(mtx);
                strcpy(response, "Ok\n");
            } else if (match_cmd(cmd, "show")) {
                sprintf(response, "%ld\n", counter);
            } else {
                strcpy(response, "Unknown command\n");
            }
            
            if (write(con->socket, response, strlen(response)) == -1) {
                perror("write");
                close(con->socket);
                close(srv->socket);
                exit(1);
            }

            printf("Command received: %s\n", cmd);
        }
    }

    return NULL;
}

void server_run(struct server *srv)
{
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    sem_t sem;
    
    struct connection_handler_data data;
    data.srv = srv;
    data.mtx = &mtx;
    data.sem = &sem;
    sem_init(&sem, 0, 0);

    while (1) {
        pthread_t th;
        int socket;
        struct connection *con;
        if ((socket = accept(srv->socket, NULL, NULL)) == -1) {
            perror("accept");
            close(srv->socket);
            exit(1);
        }

        con = connection_create(socket);
        dynamic_array_append(srv->connections, &con);

        printf("New client connected\n");
        server_print_info(srv);

        data.con = con;
        pthread_create(&th, NULL, connection_handler, &data);
        pthread_detach(th);
        sem_wait(&sem);
    }

    sem_destroy(&sem);
    close(srv->socket);
}

void server_destroy(struct server *srv)
{
    free(srv->connections->ptr);
    free(srv->connections);
    free(srv);
}
