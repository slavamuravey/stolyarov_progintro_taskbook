#include <stdio.h>

size_t my_strlen(const char *s)
{
    const char *p;
    for (p = s; *p; p++) {}

    return p - s;
}

int main(int argc, char **argv)
{
    char *max_arg;
    size_t arg_max_len = 0;

    if (argc == 1) {
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        size_t arg_len = my_strlen(arg);
        if (arg_len > arg_max_len) {
            arg_max_len = arg_len;
            max_arg = arg;
        }
    }

    printf("a) Max length argument is '%s', its length is %ld\n", max_arg, arg_max_len);

    return 0;
}
