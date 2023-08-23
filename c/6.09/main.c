#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>

int main(int argc, char **argv)
{
    printf("What is your name, please?\n");

    int max_d = STDIN_FILENO;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;

    int sr = select(max_d + 1, &readfds, NULL, NULL, &timeout);
    if (sr == -1) {
        if (errno != EINTR) {
            perror("select");
            exit(1);
        }

        exit(0);
    } else if (sr == 0) {
        printf("Sorry, I'm terribly busy.\n");
        exit(0);
    } else {
        size_t cap = 2;
        size_t len = 0;
        char *name = malloc(cap * sizeof(char));
        int c;
        while ((c = getchar()) != EOF) {
            if (len + 1 == cap) {
                cap *= 2;
                name = realloc(name, cap * sizeof(char));
            }

            if (c == '\n') {
                break;
            }

            name[len] = c;

            len++;
        }

        name[len] = '\0';

        printf("Nice to meet you, dear %s!\n", name);
        free(name);
    }

    return 0;
}
