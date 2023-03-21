#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv)
{
    char *text = 
        "Humpty Dumpty sat on a wall,\n"
        "Humpty Dumpty had a great fall.\n"
        "Four-score Men and Four-score more,\n"
        "Could not make Humpty Dumpty where he was before.\n"
    ;

    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    char *filename = argv[1];
    FILE *f = fopen(filename, "a");
    if (!f) {
        perror(filename);
        exit(1);
    }

    for (; *text; text++) {
        fputc(*text, f);
    }

    fclose(f);

    return 0;
}
