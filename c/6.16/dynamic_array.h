#ifndef DYNAMIC_ARRAY_SENTRY
#define DYNAMIC_ARRAY_SENTRY

#include <unistd.h>

struct dynamic_array {
    size_t len;
    size_t cap;
    size_t width;
    void *ptr;
};

struct dynamic_array *dynamic_array_create(size_t cap, size_t width);
void dynamic_array_append(struct dynamic_array *da, void *ptr);
void dynamic_array_slice(struct dynamic_array *da, size_t from_index, size_t to_index);
void dynamic_array_remove(struct dynamic_array *da, size_t index);

#endif
