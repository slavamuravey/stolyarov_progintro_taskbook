#include <stdio.h>
#include <stdlib.h>

struct item {
    int x;
    struct item *next;
};

struct item *array_to_list(const int *array, size_t size)
{
    struct item *first = NULL, *tmp;
    
    for (int i = size - 1; i >= 0; i--) {
        tmp = malloc(sizeof(struct item));
        tmp->x = array[i];
        tmp->next = first;
        first = tmp;
    }

    return first;
}

struct item *array_to_list_rec(const int *array, size_t size)
{
    if (!size) {
        return NULL;
    }

    struct item *tmp;
    tmp = malloc(sizeof(struct item));
    tmp->x = *array;
    tmp->next = array_to_list_rec(++array, --size);

    return tmp;
}

int *list_to_array(struct item *first)
{
    size_t size = 0;
    struct item *tmp;

    for (tmp = first; tmp; tmp = tmp->next) {
        size++;
    }

    int *array = malloc(size + 1);
    *array = size;
    int i = 0;
    for (tmp = first; tmp; tmp = tmp->next, i++) {
        array[i + 1] = tmp->x;
    }

    return array;
}

int main()
{
    int array[] = {1, 2, 3};
    size_t array_size;
    array_size = sizeof(array) / sizeof(*array);
    struct item* list_item;
    
    list_item = array_to_list(array, array_size);

    printf("%d\n", list_item->x);
    printf("%d\n", list_item->next->x);
    printf("%d\n", list_item->next->next->x);

    list_item = array_to_list_rec(array, array_size);

    printf("%d\n", list_item->x);
    printf("%d\n", list_item->next->x);
    printf("%d\n", list_item->next->next->x);

    int *array_from_list = list_to_array(list_item);

    array_size = *array_from_list;
    for (int i = 0; i < array_size; i++) {
        printf("%d\n", *(array_from_list + i + 1));
    }

    return 0;
}
