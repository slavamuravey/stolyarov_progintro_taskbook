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

int main()
{
    int c;
    int is_word;
    struct word *first, *tmp;
    struct dynamic_string *word;
    first = NULL;
    word = NULL;

    is_word = 0;
    while ((c = getchar()) != EOF) {
        if (isspace(c) || c == '\n') {
            if (is_word) {
                word_list_item_add(&first, word);
                is_word = 0;
            }
        } else {
            if (!is_word) {
                word = dynamic_string_create(4);
            }
            dynamic_string_append(word, c);
            is_word = 1;
        }

        if (c == '\n') {
            for (tmp = first; tmp; tmp = tmp->next) {
                printf("%s", tmp->word->p);

                if (tmp->next) {
                   putchar(' '); 
                }
            }

            putchar('\n');

            word_list_destroy(&first);
        }
    }

    return 0;
}
