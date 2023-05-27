#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    signal(SIGPIPE, SIG_IGN);

    int fd[2];
    pipe(fd);
    int fd_read = fd[0];
    int fd_write = fd[1];
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        close(fd_write);
        dup2(fd_read, STDIN_FILENO);
        close(fd_read);
        
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

    close(fd_read);
    
    FILE *write_stream = fdopen(fd_write, "w");
    
    int i;
    for (i = 1; i <= 100000; i++) {
        if (fprintf(write_stream, "%d\n", i) < 0) {
            perror("fprintf");
        }
    }

    fclose(write_stream);
    
    int status;
    wait(&status);

    if (WIFEXITED(status)) {
        printf("exited %d\n", WEXITSTATUS(status));
    } else {
        printf("killed %d\n", WTERMSIG(status));
    }

    return 0;
}
