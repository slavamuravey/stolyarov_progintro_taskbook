#include <stdlib.h>
#include "dynamic_array.h"
#include "connection.h"

struct connection *connection_create(int socket)
{
    struct connection *con = malloc(sizeof(struct connection));
    con->socket = socket;
    con->buf = dynamic_array_create(BUFSIZE, sizeof(char));

    return con;
}

void connection_destroy(struct connection *con)
{
    free(con->buf->ptr);
    free(con->buf);
    free(con);
}
