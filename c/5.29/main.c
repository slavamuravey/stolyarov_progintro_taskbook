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

    int fd_in[2];
    pipe(fd_in);
    int fd_in_read = fd_in[0];
    int fd_in_write = fd_in[1];

    int fd_out[2];
    pipe(fd_out);
    int fd_out_read = fd_out[0];
    int fd_out_write = fd_out[1];

    pid_t pid_in = fork();
    if (pid_in == -1) {
        perror("fork");
        exit(1);
    }

    if (pid_in == 0) {
        close(fd_in_write);
        dup2(fd_in_read, STDIN_FILENO);
        close(fd_in_read);

        close(fd_out_read);
        dup2(fd_out_write, STDOUT_FILENO);
        close(fd_out_write);
        
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

    pid_t pid_out = fork();
    if (pid_out == -1) {
        perror("fork");
        exit(1);
    }

    if (pid_out == 0) {
        close(fd_in_read);
        close(fd_in_write);
        close(fd_out_write);
        FILE *read_stream = fdopen(fd_out_read, "r");
        size_t chars_count = 0;
        size_t lines_count = 0;
        int c;
        while ((c = fgetc(read_stream)) != EOF) {
            chars_count++;
            if (c == '\n') {
                lines_count++;
            }
        }
        printf("chars_count: %ld, lines_count: %ld\n", chars_count, lines_count);
        fclose(read_stream);
        exit(0);
    }

    close(fd_in_read);
    close(fd_out_read);
    close(fd_out_write);

    FILE *write_stream = fdopen(fd_in_write, "w");
    
    int i;
    for (i = 1; i <= 100000; i++) {
        if (fprintf(write_stream, "%d\n", i) < 0) {
            perror("fprintf");
        }
    }

    fclose(write_stream);
    
    wait(NULL);
    wait(NULL);

    return 0;
}
