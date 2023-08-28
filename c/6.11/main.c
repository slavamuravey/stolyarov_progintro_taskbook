#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    char *r_filename = argv[1];
    char *w_filename = argv[2];

    int r_fd = open(r_filename, O_RDONLY | O_NONBLOCK);
    if (r_fd == -1) {
        perror("open");
        exit(1);
    }
    int w_fd = open(w_filename, O_WRONLY);
    if (w_fd == -1) {
        perror("open");
        exit(1);
    }
    
    printf("Connection established.\n");

    int max_d = r_fd;
    
    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(r_fd, &readfds);
        int sr = select(max_d + 1, &readfds, NULL, NULL, NULL);
        if (sr == -1) {
            if (errno != EINTR) {
                perror("select");
                exit(1);
            }

            continue;
        } else if (sr == 0) {
            continue;
        } else {
            if (FD_ISSET(STDIN_FILENO, &readfds)) {
                char buf[4];
                ssize_t count = read(STDIN_FILENO, buf, sizeof(buf));
                if (count == -1) {
                    perror("read");
                    exit(1);
                }

                if (count == 0) {
                    exit(0);
                } else {
                    write(w_fd, buf, count);
                }
            }
            if (FD_ISSET(r_fd, &readfds)) {
                char buf[4];
                ssize_t count = read(r_fd, buf, sizeof(buf));
                if (count == -1) {
                    perror("read");
                    exit(1);
                }

                if (count == 0) {
                    printf("Connection closed.\n");
                    exit(0);
                } else {
                    write(STDIN_FILENO, buf, count);
                }
            }
        }
    }

    return 0;
}
