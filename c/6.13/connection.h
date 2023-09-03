#ifndef CONNECTION_SENTRY
#define CONNECTION_SENTRY

#include "dynamic_array.h"

struct connection {
    int socket;
    struct dynamic_array *buf;
};

struct connection *connection_create(int socket);
void connection_destroy(struct connection *con);

#endif
