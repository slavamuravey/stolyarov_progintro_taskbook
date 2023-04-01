#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define __USE_LARGEFILE64
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    long pos = lseek64(fd, 0, SEEK_END);
    printf("Bytes count: %ld\n", pos);

    close(fd);

    return EXIT_SUCCESS;
}
