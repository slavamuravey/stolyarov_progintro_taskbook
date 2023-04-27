#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

volatile static sig_atomic_t sigint_count = 0;

void sigint_handler(int s)
{
    signal(SIGINT, sigint_handler);
    sigint_count++;
}

void sigusr1_handler(int s)
{
    signal(SIGUSR1, sigusr1_handler);
    char sigint_count_str[11];
    sprintf(sigint_count_str, "%d\n", sigint_count);
    int save_errno = errno;
    write(STDOUT_FILENO, sigint_count_str, strlen(sigint_count_str));
    errno = save_errno;
}

int main(int argc, char **argv)
{
    signal(SIGINT, sigint_handler);
    signal(SIGUSR1, sigusr1_handler);

    printf("pid: %d\n", getpid());

    while (1) {
        pause();
    }

    return 0;
}
