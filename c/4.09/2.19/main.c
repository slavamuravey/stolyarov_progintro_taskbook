#include <stdio.h>

int main()
{
    int c;
    int wc = 0;
    int even_len_wc = 0;
    int odd_len_wc = 0;
    int word_len = 0;
    int is_word = 0;
    int g7_len_wc = 0;
    int le2_len_wc = 0;

    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (is_word == 1) {
                wc++;
            }

            if (word_len != 0) {
                if (word_len % 2 == 0) {
                    even_len_wc++;
                } else {
                    odd_len_wc++;
                }
            }

            if (word_len > 7) {
                g7_len_wc++;
            }

            if (word_len > 0 && word_len <= 2) {
                le2_len_wc++;
            }
            
            word_len = 0;
            is_word = 0;
        } else {
            word_len++;
            is_word = 1;
        }

        if (c == '\n') {
            printf("a) words count = %d\n", wc);
            printf("b) even length words count = %d, odd length words count = %d\n", even_len_wc, odd_len_wc);
            printf("c) greater 7 length words count = %d, lower or equal 2 length words count = %d\n", g7_len_wc, le2_len_wc);
            wc = 0;
            even_len_wc = 0;
            odd_len_wc = 0;
            g7_len_wc = 0;
            le2_len_wc = 0;
        }
    }

    return 0;
}
