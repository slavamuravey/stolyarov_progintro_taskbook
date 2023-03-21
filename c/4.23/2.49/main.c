#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    char *filename = argv[1];
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror(filename);
        exit(1);
    }

    int c;
    while ((c = getchar()) != EOF) {
        fputc(c, f);
    }

    fclose(f);

    return 0;
}
