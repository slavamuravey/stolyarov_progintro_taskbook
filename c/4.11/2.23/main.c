#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char *str, *p;
    size_t wc;
    int is_word;

    if (argc != 2) {
        fprintf(stderr, "Pass only one argument.\n");
        exit(1);
    }

    wc = 0;
    str = argv[1];
    is_word = 0;

    for (p = str; *p; p++) {
        if (isspace(*p) || *p == '\n') {
            if (is_word) {
                is_word = 0;
                wc++;
            }
        } else {
            is_word = 1;
        }
    }

    if (is_word) {
        wc++;
    }

    printf("%ld\n", wc);

    return 0;
}
