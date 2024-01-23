#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

struct number {
    int number;
    struct number *next;
};

void number_list_item_add(struct number **first, int number)
{
    struct number *item = malloc(sizeof(struct number));
    item->next = *first;
    item->number = number;
    *first = item;
}

void number_list_destroy(struct number **first)
{
    while (*first) {
        struct number *tmp;
        tmp = *first;
        *first = (*first)->next;
        free(tmp);
    }
}

int main()
{
    int number;
    int matches;
    struct number *first, *tmp;
    first = NULL;

    while (1) {
        matches = scanf("%d", &number);
        if (matches == EOF) {
            if (errno) {
                perror("scanf");
                exit(1);
            }
            break;
        }

        if (!matches) {
            break;
        }

        number_list_item_add(&first, number);
    }

    for (tmp = first; tmp; tmp = tmp->next) {
        if (tmp->next && abs(tmp->next->number - tmp->number) <= 5) {
            printf("(%d %d)", tmp->next->number, tmp->number);
        }
    }

    number_list_destroy(&first);

    return 0;
}
