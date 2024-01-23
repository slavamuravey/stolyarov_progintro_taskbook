#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct dynamic_string {
    size_t len;
    size_t cap;
    char *p;
};

struct dynamic_string *dynamic_string_create(size_t cap)
{
    struct dynamic_string *s = malloc(sizeof(struct dynamic_string));
    s->len = 1;
    s->cap = cap > s->len ? cap : s->len;
    s->p = malloc(sizeof(char) * s->cap);
    s->p[0] = '\0';

    return s;
}

void dynamic_string_append(struct dynamic_string *s, char c)
{
    s->len++;

    if (s->len > s->cap) {
        s->cap *= 2;
        struct dynamic_string *p = malloc(sizeof(char) * s->cap);
        memcpy(p, s->p, s->len - 1);
        free(p);
    }
    
    s->p[s->len - 2] = c;
    s->p[s->len - 1] = '\0';
}

void dynamic_string_destroy(struct dynamic_string *s)
{
    free(s->p);
    free(s);
}

struct word {
    struct dynamic_string *word;
    struct word *next;
};

void word_list_item_add(struct word **first, struct dynamic_string *word)
{
    struct word *item = malloc(sizeof(struct word));
    item->next = *first;
    item->word = word;
    *first = item;
}

void word_list_destroy(struct word **first)
{
    while (*first) {
        struct word *tmp;
        tmp = *first;
        *first = (*first)->next;
        if (tmp->word) {
            dynamic_string_destroy(tmp->word);
        }
        free(tmp);
    }
}

size_t get_max_digits_seq_len(struct word *first)
{
    struct word *tmp;
    size_t max_len;
    max_len = 0;
    for (tmp = first; tmp; tmp = tmp->next) {
        size_t word_len = tmp->word->len - 1;
        if (word_len > max_len) {
            max_len = word_len;
        }
    }

    return max_len;
}

int main()
{
    int c;
    int is_digit;
    struct word *first, *tmp;
    struct dynamic_string *word;
    first = NULL;
    word = NULL;

    is_digit = 0;
    while ((c = getchar()) != EOF) {
        if (!isdigit(c) || c == '\n') {
            if (is_digit) {
                word_list_item_add(&first, word);
                is_digit = 0;
            }
        } else {
            if (!is_digit) {
                word = dynamic_string_create(4);
            }
            dynamic_string_append(word, c);
            is_digit = 1;
        }

        if (c == '\n') {
            size_t max_digits_seq_len;
            max_digits_seq_len = get_max_digits_seq_len(first);
            for (tmp = first; tmp; tmp = tmp->next) {
                if (tmp->word->len - 1 == max_digits_seq_len) {
                    printf("%s", tmp->word->p);

                    if (tmp->next) {
                        putchar(' '); 
                    }
                }
            }

            putchar('\n');

            word_list_destroy(&first);
        }
    }

    return 0;
}
