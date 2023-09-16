#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "dynamic_array.h"

struct dynamic_array *dynamic_array_create(size_t cap, size_t width)
{
    struct dynamic_array *da = malloc(sizeof(struct dynamic_array));
    da->len = 0;
    da->cap = cap;
    da->width = width;
    da->ptr = malloc(da->width * cap);

    return da;
}

void dynamic_array_append(struct dynamic_array *da, void *ptr)
{
    da->len++;

    if (da->len > da->cap) {
        da->cap *= 2;
        da->ptr = realloc(da->ptr, da->width * da->cap);
    }

    memcpy((char *)da->ptr + (da->len - 1) * da->width, ptr, da->width);
}

void dynamic_array_slice(struct dynamic_array *da, size_t from_index, size_t to_index)
{
    int slice_len;

    if (from_index > da->len - 1) {
        from_index = da->len - 1;
    }

    if (to_index > da->len) {
        to_index = da->len;
    }

    slice_len = to_index - from_index;
    if (slice_len < 0) {
        slice_len = 0;
    }
    da->len = slice_len;
    memmove(da->ptr, (char *)da->ptr + (from_index * da->width), slice_len * da->width);
}

void dynamic_array_remove(struct dynamic_array *da, size_t index)
{
    da->len--;
    memmove((char *)da->ptr + index * da->width, (char *)da->ptr + (index + 1) * da->width, (da->len - index) * da->width);
}