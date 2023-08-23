#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/select.h>

#define SECONDS_TO_ALARM 5

volatile static sig_atomic_t chars_count = 0, lines_count = 0;
volatile static sig_atomic_t need_exit = false;

void sigint_handler(int s)
{
    signal(s, sigint_handler);

    if (need_exit) {
        exit(0);
    }
    need_exit = true;
    char statistic_str[100];
    sprintf(statistic_str, "chars count: %d, lines count: %d\n", chars_count, lines_count);
    int save_errno = errno;
    write(STDOUT_FILENO, statistic_str, strlen(statistic_str));
    errno = save_errno;
}

int main(int argc, char **argv)
{
    signal(SIGINT, sigint_handler);

    fd_set readfds;
    struct timeval timeout;
    int max_d = STDIN_FILENO;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        timeout.tv_sec = SECONDS_TO_ALARM;
        timeout.tv_usec = 0;

        int sr = select(max_d + 1, &readfds, NULL, NULL, &timeout);
        if (sr == -1) {
            if (errno != EINTR) {
                perror("select");
                exit(1);
            }

            continue;
        } else if (sr == 0) {
            need_exit = false;
            printf("Are you sleeping?\n");
        } else {
            int c;
            while ((c = getchar()) != EOF) {
                need_exit = false;
                chars_count++;
                if (c == '\n') {
                    lines_count++;
                    break;
                }
            }
        }
    }

    return 0;
}
