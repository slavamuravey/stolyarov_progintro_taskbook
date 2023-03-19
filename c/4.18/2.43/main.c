#include <stdio.h>
#include <stdlib.h>
#include "../lib/my_string.h"

struct word {
    char *str;
    size_t str_len;
    struct word *next;
};

int get_word_max_len(struct word *first)
{
    int max_len = 0;
    for (; first; first = first->next) {
        if (first->str_len > max_len) {
            max_len = first->str_len;
        }
    }

    return max_len;
}

void free_words_list(struct word *first)
{
    struct word *tmp;
    while (first) {
        tmp = first;
        first = first->next;
        free(tmp->str);
        free(tmp);
    }
}

int main(int argc, char **argv)
{
    int c;
    int is_word = 0;
    char *word;
    struct word *first = NULL, *last = NULL, *tmp;

    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (is_word == 1) {
                tmp = malloc(sizeof(struct word));
                tmp->str = word;
                tmp->str_len = my_strlen(word);
                tmp->next = NULL;

                if (last) {
                    last->next = tmp;
                    last = last->next;
                } else {
                    first = last = tmp;
                }
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
            size_t word_max_len = get_word_max_len(first);
            for (int i = 0; i < word_max_len; i++) {
                struct word *tmp = first;
                for (; tmp; tmp = tmp->next) {
                    if (i < tmp->str_len) {
                        putchar(tmp->str[i]);
                    } else {
                        putchar(' ');
                    }

                    putchar(' ');
                }
                putchar('\n');
            }

            free_words_list(first);
            first = last = NULL;
        }
    }

    return 0;
}
