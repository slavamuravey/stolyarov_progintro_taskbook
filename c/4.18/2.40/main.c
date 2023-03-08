#include <stdio.h>
#include <stdlib.h>

struct item {
    long int number;
    unsigned int hits;
    struct item *next;
};

struct item *create_item(long int number)
{
    struct item *tmp;
    tmp = malloc(sizeof(struct item));
    tmp->number = number;
    tmp->hits = 1;
    tmp->next = NULL;

    return tmp;
}

struct item *collect_number(struct item *first, long int number)
{
    if (first == NULL) {
        return create_item(number);
    }

    struct item *tmp = first;

    while (1) {
        if (tmp->number == number) {
            tmp->hits++;

            return first;
        }

        if (tmp->next == NULL) {
            tmp->next = create_item(number);

            return first;
        }

        tmp = tmp->next;
    }
}

int main(int argc, char **argv)
{
    long int number;
    struct item *first = NULL, *tmp;

    while (scanf("%ld", &number) > 0) {
        first = collect_number(first, number);
    }

    for (tmp = first; tmp; tmp = tmp->next) {
        if (tmp->hits == 3) {
            printf("%ld\n", tmp->number);
        }
    }

    return 0;
}
