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
        close(fd_read);
        dup2(fd_write, STDOUT_FILENO);
        close(fd_write);
        
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

    close(fd_write);
    
    FILE *read_stream = fdopen(fd_read, "r");
    
    size_t counter = 0;
    int c;
    while ((c = fgetc(read_stream)) != EOF) {
        counter++;
        if (counter <= 20) {
            putchar(c);
        }
        if (c == '\n') {
            counter = 0;
            putchar(c);
        } 
    }

    fclose(read_stream);
    
    wait(NULL);

    return 0;
}
