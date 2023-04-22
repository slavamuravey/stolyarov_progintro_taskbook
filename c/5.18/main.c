#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    char *filename = argv[1];
    char *prog = argv[2];
    char **prog_argv = argv + 2;

    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd == -1) {
        perror(filename);
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        dup2(fd, 1);
        close(fd);

        execvp(prog, prog_argv);
        perror(prog);
        exit(1);
    }

    close(fd);

    wait(NULL);

    return 0;
}
