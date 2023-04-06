#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];

    int fd = open(filename, O_RDONLY);

    char buf[4096];

    ssize_t lines_count = 0;

    ssize_t read_count;
    while ((read_count = read(fd, buf, sizeof(buf)))) {
        for (int i = 0; i < read_count; i++) {
            if (buf[i] == '\n') {
                lines_count++;
            }
        }
    }

    printf("%ld\n", lines_count);

    close(fd);

    return 0;
}
