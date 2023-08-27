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

#define LISTEN_BACKLOG 16

struct connections {
    size_t len;
    size_t cap;
    int *ptr;
};

struct connections *connections_create(size_t cap)
{
    struct connections *cons = malloc(sizeof(struct connections));
    cons->len = 0;
    cons->cap = cap;
    cons->ptr = malloc(sizeof(int) * cap);

    return cons;
}

void connections_append(struct connections *cons, int con)
{
    cons->len++;

    if (cons->len > cons->cap) {
        cons->cap *= 2;
        cons->ptr = realloc(cons->ptr, sizeof(int) * cons->cap);
    }

    cons->ptr[cons->len - 1] = con;
}

void connections_destroy(struct connections *cons)
{
    free(cons->ptr);
    free(cons);
}

int main(int argc, char **argv) 
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    int port = atoi(argv[1]);
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    int ls = socket(AF_INET, SOCK_STREAM, 0);
    if (ls == -1) {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    int opt = 1;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(ls, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(ls);
        exit(1);
    }

    if (listen(ls, LISTEN_BACKLOG) == -1) {
        perror("listen");
        close(ls);
        exit(1);
    }

    struct connections *cons = connections_create(2);

    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(ls, &readfds);
        int max_d = ls;
        int i;
        for (i = 0; i < cons->len; i++) {
            int con = cons->ptr[i];
            if (con > 0) {
                FD_SET(con, &readfds);
                if (con > max_d) {
                    max_d = con;
                }
            }
        }

        int sr = select(max_d + 1, &readfds, NULL, NULL, NULL);

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

        if (FD_ISSET(ls, &readfds)) {
            int con;
            if ((con = accept(ls, NULL, NULL)) == -1) {
                perror("accept");
                close(ls);
                exit(1);
            }

            connections_append(cons, con);
        }

        for (i = 0; i < cons->len; i++) {
            int con = cons->ptr[i];
            if (FD_ISSET(con, &readfds)) {
                size_t count_total = 0;
                size_t count;
                char buf[4];
                while ((count = read(con, buf, sizeof(buf))) > 0) {
                    int j;
                    int eol = 0;
                    for (j = 0; j < count; j++) {
                        if (buf[j] == '\n') {
                            eol = 1;
                            break;
                        }
                    }

                    count_total += count;

                    if (eol) {
                        break;
                    }
                }

                if (count_total == 0) {
                    cons->ptr[i] = -1;
                    shutdown(con, SHUT_RDWR);
                    close(con);
                    break;
                }

                char *response = "Ok\n";
                if (write(con, response, strlen(response)) == -1) {
                    perror("write");
                    close(con);
                    close(ls);
                    exit(1);
                }
            }
        }
    }
    
    close(ls);

    return 0;
}
