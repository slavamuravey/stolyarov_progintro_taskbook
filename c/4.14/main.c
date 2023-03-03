#include <stdio.h>

void filter_char(char *haystack, char needle)
{
    char *src;
    char *dst;
    src = dst = haystack;

    while (1) {
        if (*src != needle) {
            *dst = *src;
            if (!*src) {
                return;
            }
            
            dst++;
        }
        src++;
    }
}

int main()
{
    char s[] = " asd  w ";
    filter_char(s, ' ');
    printf("\"%s\"\n", s);

    return 0;
}
