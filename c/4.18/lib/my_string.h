#ifndef MY_STRING_H_SENTRY
#define MY_STRING_H_SENTRY

#include <stdlib.h>

size_t my_strlen(const char *s);
char *my_strcpy(char *dest, const char *src);
char *my_stralloc(const char *src, size_t size);
void append_char(char *dest, char ch);

#endif
