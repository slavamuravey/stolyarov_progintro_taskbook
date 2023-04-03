#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char buffer[1024];
    ssize_t count;
    while ((count = read(STDIN_FILENO, buffer, sizeof(buffer)))) {
        write(STDOUT_FILENO, buffer, count);
    }

    return 0;
}
