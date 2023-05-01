#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

enum symbol {
    plus,
    minus
};

volatile static sig_atomic_t toggler = plus;
volatile static sig_atomic_t need_exit = false;
static const char *plus_str = "+";
static const char *minus_str = "-";

void sig_handler(int s)
{
    signal(s, sig_handler);
    alarm(1);

    if (s == SIGALRM) {
        need_exit = false;
    }

    if (s == SIGINT) {
        if (need_exit) {
            exit(0);
        }
        need_exit = true;
        toggler = minus;
    }

    if (s == SIGQUIT) {
        need_exit = false;
        toggler = plus;
    }

    int save_errno = errno;
    const char *symbol_str = toggler == plus ? plus_str : minus_str;
    write(STDOUT_FILENO, symbol_str, strlen(symbol_str));
    errno = save_errno;
}

int main(int argc, char **argv)
{
    signal(SIGALRM, sig_handler);
    signal(SIGINT, sig_handler);
    signal(SIGQUIT, sig_handler);
    alarm(1);

    while (1) {
        pause();
    }

    return 0;
}
