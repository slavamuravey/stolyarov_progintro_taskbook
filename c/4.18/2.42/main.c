#include <stdio.h>
#include <stdlib.h>
#include "../lib/my_string.h"

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
                word = my_stralloc("", 1 + 1);
            } else {
                char *new_word = my_stralloc(word, my_strlen(word) + 1 + 1);
                free(word);
                word = new_word;
                // word = realloc(word, my_strlen(word) + 1 + 1);
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
