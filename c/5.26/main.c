#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    char *argv1[argc], *argv2[argc];
    char **argvp = argv1;
    *argv2 = NULL;

    int i;
    for (i = 1; i < argc; i++, argvp++) {
        if (!strcmp(argv[i], ";;") && *argv2 == NULL) {
            *argvp = NULL;
            argvp = argv2 - 1;
            continue;
        }
        *argvp = argv[i];
    }

    *argvp = NULL;

    int fd[2];
    pipe(fd);

    int fd_read = fd[0];
    int fd_write = fd[1];

    if (fork() == 0) {
        close(fd_read);
        dup2(fd_write, STDOUT_FILENO);
        close(fd_write);
        execvp(*argv1, argv1);
        perror(*argv1);
        exit(1);
    }

    if (fork() == 0) {
        close(fd_write);
        dup2(fd_read, STDIN_FILENO);
        close(fd_read);
        execvp(*argv2, argv2);
        perror(*argv2);
        exit(1);
    }

    close(fd_read);
    close(fd_write);

    wait(NULL);
    wait(NULL);

    return 0;
}
