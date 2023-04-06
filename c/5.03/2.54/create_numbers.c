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
    int fd = open(filename, O_WRONLY | O_CREAT, 0666);

    int number;
    while (scanf("%d", &number) > 0) {
        write(fd, &number, sizeof(int));
    }

    close(fd);

    return 0;
}
