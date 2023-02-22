#include <stdio.h>

unsigned int get_spaces_count(const char *s)
{
    unsigned int count = 0;

    for (const char *p = s; *p; p++) {
        if (*p == ' ') {
            count++;
        }
    }

    return count;
}

int main()
{
    char *s = "Hel lo world ";
    printf("get_spaces_count result for \"%s\" = %u\n", s, get_spaces_count(s));

    return 0;
}
