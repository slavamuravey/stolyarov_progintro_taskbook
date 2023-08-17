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

void demonize()
{
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_WRONLY);
    chdir("/");

    pid_t pid;
    
    /* fork process to change pid before setsid */
    pid = fork();
    if (pid == -1) {
        perror("fork");
    }

    if (pid > 0) {
        exit(0);
    }

    setsid();

    /* fork process to stop be session leader */
    pid = fork();
    if (pid == -1) {
        perror("fork");
    }

    if (pid > 0) {
        exit(0);
    }
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
        close(s);
        exit(1);
    }

    demonize();
    
    openlog("UPD daemon", 0, LOG_USER);
    syslog(LOG_INFO, "Daemon started, pid=%d", getpid());

    struct sockaddr_in addr_from;
    int bytes_received = 0;
    int dgrams_count = 0;

    while (1) {
        memset(&addr_from, 0, sizeof(addr_from));

        socklen_t addr_from_len = sizeof(addr_from);

        char buf[508];
        memset(buf, 0, sizeof(buf));
        int byte_count;
        if ((byte_count = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&addr_from, &addr_from_len)) == -1) {
            syslog(LOG_ERR, "recvfrom: %s", strerror(errno));
            close(s);
            exit(1);
        }
        bytes_received += byte_count;
        dgrams_count++;

        char msg[508];
        sprintf(msg, "bytes received: %d, dgrams count: %d\n", bytes_received, dgrams_count);
        if (sendto(s, msg, strlen(msg), 0, (struct sockaddr *)&addr_from, sizeof(addr_from)) == -1) {
            syslog(LOG_ERR, "sendto: %s", strerror(errno));
            close(s);
            exit(1);
        }
    }

    closelog();
    close(s);

    return 0;
}
