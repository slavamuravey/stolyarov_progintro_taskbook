#include "my_string.h"

size_t my_strlen(const char *s)
{
    const char *p;
    for (p = s; *p; p++) {}

    return p - s;
}

char *my_strcpy(char *dest, const char *src)
{
    char *p = dest;
    for (; *src; src++, p++) {
        *p = *src;
    }

    *p = '\0';

    return dest;
}

char *my_stralloc(const char *src, size_t size)
{
    char *dest = malloc(size);
    my_strcpy(dest, src);

    return dest;
}

void append_char(char *dest, char ch)
{
    size_t len = my_strlen(dest);
    dest[len] = ch;
    dest[len + 1] = '\0';
}
