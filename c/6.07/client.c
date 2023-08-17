#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <errno.h>

volatile static sig_atomic_t sock;

void sigalrm_handler(int s)
{
    signal(SIGALRM, sigalrm_handler);

    int save_errno = errno;
    char *time_out_msg = "Response waiting time out.\n";
    write(STDOUT_FILENO, time_out_msg, strlen(time_out_msg));
    errno = save_errno;

    close(sock);
    exit(0);
}

int main(int argc, char **argv) 
{
    if (argc < 4) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    char *msg = argv[3];
    
    struct sockaddr_in addr_to;
    memset(&addr_to, 0, sizeof(addr_to));
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(1);
    }

    addr_to.sin_family = AF_INET;
    addr_to.sin_addr.s_addr = inet_addr(ip);
    addr_to.sin_port = htons(port);

    if (sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&addr_to, sizeof(addr_to)) == -1) {
        perror("sendto");
        close(sock);
        exit(1);
    }

    signal(SIGALRM, sigalrm_handler);

    char buf[508];
    memset(buf, 0, sizeof(buf));

    alarm(1);
    if (read(sock, buf, sizeof(buf)) == -1) {
        perror("read");
        close(sock);
        exit(1);
    }

    printf("%s\n", buf);
    close(sock);
    
    return 0;
}
