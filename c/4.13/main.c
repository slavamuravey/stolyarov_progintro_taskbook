#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

#define MAX_NATURAL_NUMBER 3037000499
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

char *build_seq(long long n, long long m)
{
    char *buf;
    size_t buf_cap, buf_len;
    long long i, perfect_square;
    long long seq_len, num_len;
    seq_len = 0;
    i = 1;
    buf_cap = 8;
    buf_len = 0;
    buf = malloc(sizeof(char) * buf_cap);
    while (1) {
        size_t limit, offset;
        perfect_square = i * i;
        char perfect_square_str[32];
        sprintf(perfect_square_str, "%lld", perfect_square);
        num_len = strlen(perfect_square_str);

        if (seq_len + num_len >= n) {
            offset = MAX(n - seq_len - 1, 0);
            limit = MIN(offset + m - seq_len, num_len - offset);
            
            if (buf_len + limit > buf_cap) {
                buf_cap *= 2;
                buf = realloc(buf, buf_cap);
                if (buf == NULL) {
                    perror("realloc");
                    exit(1);
                }
            }

            memcpy(buf + buf_len, perfect_square_str + offset, limit);
            buf_len += limit;
        }
        
        seq_len += num_len;

        if (seq_len >= m) {
            return buf;
        }
        i++;
    }

    return buf;
}

long long parse_number_lld(char *number_str)
{
    char *p;
    long long n;
    n = strtoll(number_str, &p, 10);
    if (errno != 0) {
        perror(number_str);
        exit(EXIT_FAILURE);
    }
    if (*p != '\0') {
        fprintf(stderr, "%s: %s\n", "Bad number", number_str);
        exit(EXIT_FAILURE);
    }

    return n;
}

void assert_number_natural(long long n)
{
    if (n <= 0) {
        fprintf(stderr, "%s: %lld\n", "Number should be natural", n);
        exit(1);
    }
}

void assert_number_limit(long long n)
{
    if (n > MAX_NATURAL_NUMBER) {
        fprintf(stderr, "%s: %lld\n", "Numerical result out of range", n);
        exit(1);
    }
}

int main(int argc, char **argv)
{
    char *n_str, *m_str, *res_str;
    long long n, m;
    
    if (argc < 3) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    n_str = argv[1];
    m_str = argv[2];

    n = parse_number_lld(n_str);
    assert_number_natural(n);
    assert_number_limit(n);

    m = parse_number_lld(m_str);
    assert_number_natural(m);
    assert_number_limit(m);

    if (n > m) {
        fprintf(stderr, "The second parameter should be greater then the first one.\n");
        exit(1);
    }

    res_str = build_seq(n, m);
    printf("%s\n", res_str);
    free(res_str);

    return 0;
}
