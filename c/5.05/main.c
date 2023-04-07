#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int main(int argc, char **argv)
{
    if (argc < 5) {
        fprintf(stderr, "Too few arguments.\n");
        exit(EXIT_FAILURE);
    }

    char *p;

    char *filename = argv[1];

    int fd = open(filename, O_WRONLY);
    if (fd == -1) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    long int start_pos = strtol(argv[2], &p, 10);

    if (errno != 0 || *p != '\0' || start_pos > INT_MAX || start_pos < INT_MIN) {
        if (errno != 0) {
            perror(argv[2]);
        } else {
            fprintf(stderr, "%s: %s\n", "Bad number", argv[2]);
        }
        exit(EXIT_FAILURE);
    }

    long int len = strtol(argv[3], &p, 10);

    if (errno != 0 || *p != '\0' || len > INT_MAX || len < INT_MIN) {
        if (errno != 0) {
            perror(argv[3]);
        } else {
            fprintf(stderr, "%s: %s\n", "Bad number", argv[3]);
        }
        
        exit(EXIT_FAILURE);
    }

    if (strlen(argv[4]) > 1) {
        fprintf(stderr, "%s: %s\n", "Bad character", argv[4]);
        exit(EXIT_FAILURE);
    }

    char byte = *argv[4];

    char buf[2];

    for (int i = 0; i < sizeof(buf); i++) {
        buf[i] = byte;
    }

    lseek(fd, start_pos, SEEK_SET);

    for (long int i = len; i >= 0; i -= sizeof(buf)) {
        write(fd, buf, MIN(i, sizeof(buf)));
    }

    close(fd);

    return 0;
}
