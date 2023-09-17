#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Too few arguments.\n");
        exit(EXIT_FAILURE);
    }

    char *filename;
    char *key;
    char *endptr;
    int fd;
    filename = argv[1];
    key = argv[2];
    uint32_t key_number;
    fd = open(filename, O_RDWR);
    if (fd == -1) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    key_number = strtoul(key, &endptr, 10);
    if (*endptr) {
        fprintf(stderr, "Invalid key\n");

        return 1;
    }

    int count;
    char buf[4096];
    while ((count = read(fd, buf, sizeof(buf))) != 0) {
        int i;
        off_t current_pos;
        
        for (i = 0; i < count; i++) {
            buf[i] ^= ((key_number >> (8 * (i % 4))) & 0xff);
        }

        current_pos = lseek(fd, 0, SEEK_CUR);
        lseek(fd, current_pos - count, SEEK_SET);

        if (write(fd, buf, count) == -1) {
            perror("write");
            exit(1);
        }
    }

    close(fd);

    return 0;
}
