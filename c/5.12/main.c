#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        char *prog = argv[1];
        char *new_argv[argc];
        int i;
        for (i = 1; i < argc; i++) {
            new_argv[i - 1] = argv[i];
        }
        new_argv[i - 1] = NULL;
        execvp(prog, new_argv);
        perror(prog);
        exit(1);
    }
    
    int status;
    wait(&status);

    if (WIFEXITED(status)) {
        printf("exited %d\n", WEXITSTATUS(status));
    } else {
        printf("killed %d\n", WTERMSIG(status));
    }

    return 0;
}
