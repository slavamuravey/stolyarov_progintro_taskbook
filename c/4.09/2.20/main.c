#include <stdio.h>

int main()
{
    int c;
    int is_word = 0;

    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\n') {
            if (is_word == 1) {
                putchar(')');
            }

            is_word = 0;
        } else {
            if (is_word == 0) {
                putchar('(');
            }

            putchar(c);
            
            is_word = 1;
        }

        if (c == '\n') {
            putchar(c);
        }
    }

    return 0;
}
