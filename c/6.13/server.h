#ifndef SERVER_H_SENTRY
#define SERVER_H_SENTRY

#include <sys/socket.h>
#include "dynamic_array.h"

struct server {
    int socket;
    struct dynamic_array *connections;
};

struct server *server_create(int domain, struct sockaddr *addr);
void server_run(struct server *srv);
void server_destroy(struct server *srv);

#endif
