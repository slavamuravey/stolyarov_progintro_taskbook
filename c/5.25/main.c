#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int fd[2];
    pipe(fd);

    int fd_read = fd[0];
    int fd_write = fd[1];

    printf("%d %d\n", fd_read, fd_write);
    
    if (fork() == 0) {
        close(fd_read);
        char *text[] = {
            "Humpty Dumpty sat on a wall,\n",
            "Humpty Dumpty had a great fall.\n",
            "Four-score Men and Four-score more,\n",
            "Could not make Humpty Dumpty where he was before.\n"
        };

        size_t lines_count = sizeof(text) / sizeof(*text);
        int i;
        for (i = 0; i < lines_count; i++) {
            char *line = text[i];
            write(fd_write, line, strlen(line));
            usleep(100000);
        }

        close(fd_write);

        exit(0);
    }

    close(fd_write);

    char buffer[4];
    ssize_t count;  
    while ((count = read(fd_read, buffer, sizeof(buffer)))) {
        write(STDOUT_FILENO, buffer, count);
    }

    close(fd_read);
    
    wait(NULL);

    return 0;
}
