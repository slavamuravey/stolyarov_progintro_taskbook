#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

ssize_t my_getline(char **line, size_t *buffer, FILE *stream)
{
    int c;
    int read_chars = 0;
    while ((c = fgetc(stream)) != EOF) {
        read_chars++;
        if (read_chars > *buffer - 1) {
            *buffer *= 2;
            *line = realloc(*line, *buffer);
            if (*line == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
        (*line)[read_chars - 1] = c;
        if (c == '\n') {
            break;
        }
    }

    if (read_chars == 0) {
        return EOF;
    }

    (*line)[read_chars] = '\0';

    return read_chars;
}

struct file_max_line {
    char *filename;
    char *line;
    size_t line_len;
    struct file_max_line *next;
};

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(EXIT_FAILURE);
    }

    struct file_max_line *first = NULL, *tmp;

    for (int i = 1; i < argc; i++) {
        size_t max_len = 0;

        char *filename = argv[i];
        FILE *f = fopen(filename, "r");
        if (!f) {
            perror(filename);
            exit(EXIT_FAILURE);
        }

        size_t buffer = 4;
        char *line = malloc(sizeof(char) * buffer);
        char *longest_line = malloc(sizeof(char) * buffer);
        ssize_t chars_count;
        while ((chars_count = my_getline(&line, &buffer, f)) != EOF) {
            if (chars_count > max_len) {
                max_len = chars_count;
                longest_line = realloc(longest_line, buffer);
                strcpy(longest_line, line);
            }
        }

        free(line);

        tmp = malloc(sizeof(struct file_max_line));
        tmp->filename = filename;
        tmp->line = longest_line;
        tmp->line_len = max_len;
        tmp->next = first;
        first = tmp;

        fclose(f);
    }

    size_t file_longest_line_len = 0;
    
    for (tmp = first; tmp; tmp = tmp->next) {
        if (tmp->line_len > file_longest_line_len) {
            file_longest_line_len = tmp->line_len;
        }
    }

    for (tmp = first; tmp; tmp = tmp->next) {
        if (tmp->line_len == file_longest_line_len) {
            putchar('*');
        }
        printf("%s: %s", tmp->filename, tmp->line);
        if (tmp->line[strlen(tmp->line) - 1] != '\n') {
            putchar('\n');
        }
    }

    return EXIT_SUCCESS;
}
