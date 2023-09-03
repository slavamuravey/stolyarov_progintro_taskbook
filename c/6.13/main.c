#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

int main(int argc, char **argv) 
{
    int port;
    struct server *srv;
    struct sockaddr_in addr;
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    port = atoi(argv[1]);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    srv = server_create(AF_INET, (struct sockaddr *)&addr);
    server_run(srv);
    server_destroy(srv);

    return 0;
}
