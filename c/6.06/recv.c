#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char **argv) 
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    int port = atoi(argv[1]);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1) {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(1);
    }

    struct sockaddr_in addr_from;
    memset(&addr_from, 0, sizeof(addr_from));

    socklen_t addr_from_len = sizeof(addr_from);

    char buf[508];
    memset(buf, 0, sizeof(buf));
    int byte_count;
    if ((byte_count = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&addr_from, &addr_from_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(addr_from.sin_addr), ip_str, INET_ADDRSTRLEN);
    printf("family: %d, ip: %s, port: %d, len: %d\n", addr_from.sin_family, ip_str, addr_from.sin_port, addr_from_len);

    int i;
    for (i = 0; i < byte_count; i++) {
        char c = buf[i];
        if (c < 32 || c > 126 || c == '\n' || c == '\t') {
            c = '?';
        }
        putchar(c);
    }
    putchar('\n');

    return 0;
}
