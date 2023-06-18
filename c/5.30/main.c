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

    int fd1[2];
    pipe(fd1);

    int fd1_read = fd1[0];
    int fd1_write = fd1[1];

    int fd2[2];
    pipe(fd2);

    int fd2_read = fd2[0];
    int fd2_write = fd2[1];

    if (fork() == 0) {
        close(fd2_read);
        close(fd2_write);
        close(fd1_read);
        dup2(fd1_write, STDOUT_FILENO);
        close(fd1_write);
        execvp(*argv1, argv1);
        perror(*argv1);
        exit(1);
    }

    if (fork() == 0) {
        close(fd1_write);
        close(fd2_read);

        FILE *read_stream = fdopen(fd1_read, "r");
        FILE *write_stream = fdopen(fd2_write, "w");

        bool even_line = false;
        int c;
        while ((c = fgetc(read_stream)) != EOF) {
            if (even_line) {
                fprintf(write_stream, "%c", c);
            }

            if (c == '\n') {
                even_line = !even_line;
            }
        }

        fclose(read_stream);
        fclose(write_stream);
        exit(0);
    }

    if (fork() == 0) {
        close(fd1_read);
        close(fd1_write);
        close(fd2_write);
        dup2(fd2_read, STDIN_FILENO);
        close(fd2_read);
        execvp(*argv2, argv2);
        perror(*argv2);
        exit(1);
    }

    close(fd1_read);
    close(fd1_write);

    close(fd2_read);
    close(fd2_write);

    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}
