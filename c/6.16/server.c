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
#include <sys/select.h>
#include "dynamic_array.h"
#include "server.h"
#include "connection.h"

#define LISTEN_BACKLOG 16

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

static int extract_cmd(struct dynamic_array *buf, char *cmd)
{
    int i;
    for (i = 0; i < buf->len; i++) {
        char *bytes = buf->ptr;
        if (!strncmp(bytes + i, "\r\n\r\n", 4)) {
            memcpy(cmd, bytes, i);
            dynamic_array_slice(buf, i + 4, buf->len - 1);

            return 0;
        }
    }

    return 1;
}

void server_run(struct server *srv)
{
    while (1) {
        fd_set readfds;
        int max_d;
        int i;
        int sr;
        struct connection **cons;
        FD_ZERO(&readfds);
        FD_SET(srv->socket, &readfds);
        max_d = srv->socket;
        for (i = 0; i < srv->connections->len; i++) {
            struct connection *con;
            cons = srv->connections->ptr;
            con = cons[i];
            FD_SET(con->socket, &readfds);
            if (con->socket > max_d) {
                max_d = con->socket;
            }
        }

        sr = select(max_d + 1, &readfds, NULL, NULL, NULL);

        if (sr == -1) {
            if (errno != EINTR) {
                perror("select");
                exit(1);
            }
            continue;
        }
        
        if (sr == 0) {
            continue;
        }

        if (FD_ISSET(srv->socket, &readfds)) {
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
        }

        for (i = 0; i < srv->connections->len; i++) {
            struct connection *con;
            cons = srv->connections->ptr;
            con = cons[i];
            if (FD_ISSET(con->socket, &readfds)) {
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
                    connection_close(srv, con);

                    printf("Client disconnected\n");
                    server_print_info(srv);

                    continue;
                }

                count = read(con->socket, buf + con->buf->len, nbytes);
                if (count == -1) {
                    perror("read");
                    close(con->socket);
                    close(srv->socket);
                    exit(1);
                }

                if (count == 0) {
                    connection_close(srv, con);

                    printf("Client disconnected\n");
                    server_print_info(srv);

                    continue;
                }

                con->buf->len += count;

                if (!extract_cmd(con->buf, cmd)) {
                    char response[] = 
"HTTP/1.1 200 OK\n"
"\n"
"<!DOCTYPE html>\n"
"<html>\n"
    "<body>\n"
        "<h1>My default page Heading</h1>\n"
        "<p>My default page paragraph.</p>\n"
    "</body>\n"
"</html>\n";
                    printf("Command received: %s\n", cmd);

                    if (write(con->socket, response, strlen(response)) == -1) {
                        perror("write");
                        close(con->socket);
                        close(srv->socket);
                        exit(1);
                    }

                    connection_close(srv, con);

                    printf("Client disconnected\n");
                    server_print_info(srv);
                }
            }
        }
    }
    
    close(srv->socket);
}

void server_destroy(struct server *srv)
{
    free(srv->connections->ptr);
    free(srv->connections);
    free(srv);
}
