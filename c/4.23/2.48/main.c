#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    char *filename = argv[1];
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror(filename);
        exit(1);
    }

    int line_count = 0;
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            line_count++;
        }
    }

    fclose(f);

    printf("%d\n", line_count);

    return 0;
}
