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

void word_list_item_add(struct word **first, struct word **last, struct dynamic_string *word)
{
    struct word *item = malloc(sizeof(struct word));
    item->word = word;
    item->next = NULL;
    if (*last == NULL) {
        *first = item;
        *last = *first;
    } else {
        (*last)->next = item;
        *last = item;
    }
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

void print_vertical(struct word *first)
{
    struct word *tmp;
    int counter;
    counter = 0;
    
    while (1)
    {
        int need_break;
        need_break = 1;
        for (tmp = first; tmp; tmp = tmp->next) {
            if (counter < tmp->word->len - 1) {
                printf("%c", tmp->word->p[counter]);
                need_break = 0;
            } else {
                putchar(' ');
            }
        }
        
        if (need_break) {
            putchar('\r');
            break;
        }
        
        putchar('\n');
        counter++;
    }
}

int main()
{
    int c;
    int is_word;
    struct word *first, *last;
    struct dynamic_string *word;
    first = NULL;
    last = NULL;
    word = NULL;

    is_word = 0;
    while ((c = getchar()) != EOF) {
        if (isspace(c) || c == '\n') {
            if (is_word) {
                word_list_item_add(&first, &last, word);
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
            print_vertical(first);

            word_list_destroy(&first);
            last = NULL;
        }
    }

    return 0;
}
