#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Too few arguments.\n");
        exit(EXIT_FAILURE);
    }

    char *result_filename = argv[argc - 1];
    FILE *result_file = fopen(result_filename, "w");

    for (int i = 1; i < argc - 1; i++) {
        char *filename = argv[i];
        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror(filename);
            exit(EXIT_FAILURE);
        }

        ssize_t total_numbers = 0;
        int min_number;
        int max_number;

        ssize_t count;
        int buf[2];

        while ((count = read(fd, buf, sizeof(buf)))) {
            for (int i = 0; i < count / sizeof(*buf); i++) {
                if (total_numbers == 0) {
                    min_number = max_number = buf[0];
                }

                if (buf[i] < min_number) {
                    min_number = buf[i];
                }

                if (buf[i] > max_number) {
                    max_number = buf[i];
                }

                total_numbers++;
            }
        }

        char result_str[1000];
        sprintf(result_str, "%s total numbers: %ld, min number: %d, max number: %d\n", filename, total_numbers, min_number, max_number);

        fputs(result_str, result_file);

        close(fd);
    }

    fclose(result_file);

    return 0;
}
