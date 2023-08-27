#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>

#define LISTEN_BACKLOG 16

int main(int argc, char **argv) 
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    int port = atoi(argv[1]);
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(s);
        exit(1);
    }

    if (listen(s, LISTEN_BACKLOG) == -1) {
        perror("listen");
        close(s);
        exit(1);
    }

    struct sockaddr_in peer_addr;
    socklen_t peer_addr_size = sizeof(peer_addr);

    int con;
    if ((con = accept(s, (struct sockaddr *)&peer_addr, &peer_addr_size)) == -1) {
        perror("accept");
        close(s);
        exit(1);
    }

    char buf[508];
    memset(buf, 0, sizeof(buf));

    if (read(con, buf, sizeof(buf)) == -1) {
        perror("read");
        close(con);
        close(s);
        exit(1);
    }

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(peer_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
    printf("family: %d, ip: %s, port: %d, len: %d\n", peer_addr.sin_family, ip_str, ntohs(peer_addr.sin_port), peer_addr_size);

    printf("%s\n", buf);

    if (write(con, buf, strlen(buf)) == -1) {
        perror("write");
        close(con);
        close(s);
        exit(1);
    }

    shutdown(con, SHUT_RDWR);
    close(con);
    close(s);

    return 0;
}
