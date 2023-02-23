#include <stdio.h>

unsigned int get_spaces_count(const char *s)
{
    if (!*s) {
        return 0;
    }

    return get_spaces_count(s + 1) + (*s == ' ' ? 1 : 0);
}

int main()
{
    char *s = "Hel lo world ";
    printf("get_spaces_count result for \"%s\" = %u\n", s, get_spaces_count(s));

    return 0;
}
