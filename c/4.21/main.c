#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct stackdbl *stackdbl_init();
void stackdbl_destroy(struct stackdbl *stackdbl);
void stackdbl_push(struct stackdbl *stackdbl, double value);
double stackdbl_pop(struct stackdbl *stackdbl);
bool stackdbl_empty(struct stackdbl *stackdbl);

struct stackdbl {
    double *data;
    size_t buffer;
    size_t size;
};

struct stackdbl *stackdbl_init()
{
    struct stackdbl *stackdbl = malloc(sizeof(struct stackdbl));
    stackdbl->buffer = 0;
    stackdbl->data = NULL;

    return stackdbl;
}

void stackdbl_destroy(struct stackdbl *stackdbl) 
{
    free(stackdbl->data);
    free(stackdbl);
}

void stackdbl_push(struct stackdbl *stackdbl, double value)
{
    size_t size = stackdbl->size;
    size++;

    if (size > stackdbl->buffer) {
        size_t buffer = stackdbl->size == 0 ? 1 : (stackdbl->size * 2);
        double *data = malloc(sizeof(double) * buffer);
        double *p = data;
        for (int i = 0; i < stackdbl->size; i++, stackdbl->data++, p++) {
            *p = *stackdbl->data;
        }

        stackdbl->data = data;
        stackdbl->buffer = buffer;
    }

    stackdbl->size = size;
    stackdbl->data[stackdbl->size - 1] = value;
}

double stackdbl_pop(struct stackdbl *stackdbl)
{
    if (stackdbl_empty(stackdbl)) {
        return 0;
    }

    size_t size = stackdbl->size;
    stackdbl->size--;

    return stackdbl->data[size - 1];
}

bool stackdbl_empty(struct stackdbl *stackdbl) 
{
    return stackdbl->size == 0;
}

int main(int argc, char **argv)
{
    struct stackdbl *stackdbl = stackdbl_init();

    stackdbl_push(stackdbl, 1.23);
    stackdbl_push(stackdbl, 2.23);
    stackdbl_push(stackdbl, 3.23);
    stackdbl_push(stackdbl, 4.23);
    stackdbl_push(stackdbl, 5.23);
    stackdbl_pop(stackdbl);
    stackdbl_push(stackdbl, 6.23);

    while (!stackdbl_empty(stackdbl)) {
        printf("%f\n", stackdbl_pop(stackdbl));
    }

    stackdbl_destroy(stackdbl);

    return 0;
}
