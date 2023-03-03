#include <stdio.h>
#include <string.h>

char* my_strstr(const char *haystack, const char *needle)
{
    while (*haystack) {
        const char *haystack_current = haystack; 
        const char *needle_current = needle;
        
        while (*haystack_current && *needle_current) {
            if (*haystack_current != *needle_current) {
                break;
            }
            haystack_current++;
            needle_current++;
        }

        if (!*needle_current) {
            return (char *)haystack;
        }

        haystack++;
    }

    return NULL;
}

int main(int argc, char **argv)
{
    if (argc == 1) {
        printf("Substring must be specified.\n");

        return 1;
    }

    const char *substr = argv[1];
    const unsigned long int substr_len = argv[2] - substr - 1;

    for (int i = 2; i < argc; i++) {
        unsigned long int matches_count = 0;
        const char *str = argv[i];

        while ((str = my_strstr(str, substr))) {
            str += substr_len;
            matches_count++;
        }

        if (matches_count != 0) {
            printf("%s matches: %ld\n", argv[i], matches_count);
        }        
    }

    return 0;
}
