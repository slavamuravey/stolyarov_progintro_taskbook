#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    size_t buffer_size = 1024;
    char buffer[buffer_size];
    ssize_t count;
    while ((count = read(STDIN_FILENO, buffer, buffer_size)) != 0) {
        write(STDOUT_FILENO, buffer, count);
    }

    return 0;
}
