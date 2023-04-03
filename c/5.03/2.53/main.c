#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

ssize_t my_getline(char **line, size_t *buffer, int fd)
{
    int read_chars = 0;
    char buf[1024];
    off_t current_pos = lseek(fd, 0, SEEK_CUR);
    ssize_t count;

    while ((count = read(fd, buf, sizeof(buf)))) {
        for (int i = 0; i < count; i++) {
            read_chars++;

            if (read_chars > *buffer - 1) {
                *buffer *= 2;
                *line = realloc(*line, *buffer);
                if (*line == NULL) {
                    perror("realloc");
                    exit(EXIT_FAILURE);
                }
            }

            (*line)[read_chars - 1] = buf[i];

            if (buf[i] == '\n') {
                lseek(fd, current_pos + read_chars, SEEK_SET);
                goto end_loop;
            }
        }
    }
end_loop:

    if (read_chars == 0) {
        return EOF;
    }

    (*line)[read_chars] = '\0';

    return read_chars;
}

int main(int argc, char **argv)
{
    if (argc < 4) {
        fprintf(stderr, "Too few arguments.\n");
        exit(EXIT_FAILURE);
    }

    char *src_filename = argv[1];
    char *txt_filename = argv[2];
    char *bin_filename = argv[3];

    int src_fd = open(src_filename, O_RDONLY);
    if (src_fd == -1) {
        perror(src_filename);
        exit(EXIT_FAILURE);
    }

    int txt_fd = open(txt_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (txt_fd == -1) {
        perror(txt_filename);
        exit(EXIT_FAILURE);
    }

    int bin_fd = open(bin_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (bin_fd == -1) {
        perror(bin_filename);
        exit(EXIT_FAILURE);
    }

    size_t buffer = 4;
    char *line = malloc(sizeof(char) * buffer);
    ssize_t chars_count;

    while ((chars_count = my_getline(&line, &buffer, src_fd)) != EOF) {
        if (*line == ' ') {
            write(txt_fd, line, chars_count);
        } 

        write(bin_fd, &chars_count, sizeof(size_t));
    }

    close(src_fd);
    close(txt_fd);
    close(bin_fd);

    return EXIT_SUCCESS;
}
