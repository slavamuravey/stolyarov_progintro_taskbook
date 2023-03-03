#include <stdio.h>

int main(int argc, char **argv)
{
    for (int i = 0; i < argc; i++) {
        if (*argv[i] == '-') {
            continue;
        }
        printf("%s\n", argv[i]);
    }   

    return 0;
}
