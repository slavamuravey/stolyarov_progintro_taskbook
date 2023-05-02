#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

#define SECONDS_TO_ALARM 5

const char alarm_message[] = "Are you sleeping?\n";
volatile static sig_atomic_t chars_count = 0, lines_count = 0;
volatile static sig_atomic_t need_exit = false;

void sig_handler(int s)
{
    signal(s, sig_handler);
    alarm(SECONDS_TO_ALARM);

    if (s == SIGALRM) {
        need_exit = false;
        int save_errno = errno;
        write(STDOUT_FILENO, alarm_message, sizeof(alarm_message) - 1);
        errno = save_errno;
    }

    if (s == SIGINT) {
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
}

int main(int argc, char **argv)
{
    signal(SIGALRM, sig_handler);
    signal(SIGINT, sig_handler);
    alarm(SECONDS_TO_ALARM);

    int c;
    while ((c = getchar()) != EOF) {
        need_exit = false;
        chars_count++;
        if (c == '\n') {
            lines_count++;
        }
    }

    return 0;
}
