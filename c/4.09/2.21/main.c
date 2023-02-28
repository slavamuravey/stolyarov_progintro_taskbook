#include <stdio.h>

int main()
{
    int c;
    int is_word = 0;
    int word_len = 0;
    char s[2];

    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\n') {
            if (is_word == 1) {
                if (word_len == 2) {
                     printf("%s ", s);
                }
            }

            word_len = 0;
            is_word = 0;
        } else {
            if (word_len < 2) {
                s[word_len] = c;
            }
            
            word_len++;
            is_word = 1;
        }

        if (c == '\n') {
            putchar(c);
        }
    }

    return 0;
}
