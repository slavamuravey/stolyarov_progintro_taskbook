#include <stdio.h>
#include <stdlib.h>

size_t my_strlen(const char *s)
{
    const char *p;
    for (p = s; *p; p++) {}

    return p - s;
}

void append_char(char *dest, char ch)
{
    size_t len = my_strlen(dest);
    dest[len] = ch;
    dest[len + 1] = '\0';
}

struct word {
    char *str;
    struct word *next;
};

int main(int argc, char **argv)
{
    int c;
    int is_word = 0;
    char *word;
    struct word *first = NULL, *tmp;

    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (is_word == 1) {
                tmp = malloc(sizeof(struct word));
                tmp->str = word;
                tmp->next = first;
                first = tmp;
            }

            is_word = 0;
        } else {
            if (is_word == 0) {
                word = malloc(sizeof(char) + 1);
                *word = '\0';
            } else {
                word = realloc(word, my_strlen(word) + 1);
            }

            append_char(word, c);

            is_word = 1;
        }

        if (c == '\n') {
            while (first) {
                printf("%s ", first->str);
                tmp = first;
                first = first->next;
                free(tmp->str);
                free(tmp);
            }
            putchar('\n');
        }
    }

    return 0;
}
