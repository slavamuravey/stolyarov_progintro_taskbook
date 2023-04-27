#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>

volatile static sig_atomic_t signaled = false;
static const char message[] = "Good bye\n";

void sig_handler(int s)
{
    signaled = true;
    int save_errno = errno;
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    errno = save_errno;
    exit(0);
}

int main(int argc, char **argv)
{
    printf("Press Ctrl-C to quit\n");

    signal(SIGINT, sig_handler);

    while (!signaled) {
        pause();
    }

    return 0;
}
