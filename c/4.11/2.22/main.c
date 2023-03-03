#include <stdio.h>
#include <string.h>

long unsigned int strlen(const char *s)
{
    const char *p;
    for (p = s; *p; p++) {}

    return p - s;
}

int main(int argc, char **argv)
{
    char *max_arg;
    long unsigned int arg_max_len = 0;

    if (argc == 1) {
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];
        long unsigned int arg_len = strlen(arg);
        if (arg_len > arg_max_len) {
            arg_max_len = arg_len;
            max_arg = arg;
        }
    }

    printf("a) Max length argument is '%s', its length is %ld\n", max_arg, arg_max_len);

    return 0;
}
