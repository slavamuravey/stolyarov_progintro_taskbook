#include "syscall.h"

#define MY_NULL ((void *)0)
#define MY_O_RDONLY 0
#define MY_O_WRONLY 0x241

static char *open_file_error_msg = "Open file error.\n";
static char *read_file_error_msg = "Read file error.\n";
static char *write_file_error_msg = "Write file error.\n";

unsigned int my_strlen(const char *str)
{
    const char *str_start = str;
    while (*str) {
        str++;
    }

    return str - str_start;
}

void print_error(const char *error, const char *desc)
{
    sys_write(2, error, my_strlen(error));

    if (desc) {
        sys_write(2, ": ", 2);
        sys_write(2, desc, my_strlen(desc));
    }
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        print_error("Too few arguments.\n", MY_NULL);
        sys_exit(1);
    }

    char *src_filename = argv[1];
    char *dst_filename = argv[2];

    int src_fd = sys_open(src_filename, MY_O_RDONLY);
    if (src_fd == -1) {
        print_error(src_filename, open_file_error_msg);

        sys_exit(1);
    }

    int dst_fd = sys_open(dst_filename, MY_O_WRONLY);
    if (dst_fd == -1) {
        print_error(dst_filename, open_file_error_msg);
        sys_exit(1);
    }

    char buf[4];
    int count;
    while ((count = sys_read(src_fd, buf, sizeof(buf)))) {
        if (count == -1) {
            print_error("read", read_file_error_msg);
            sys_exit(1);
        }
        if (sys_write(dst_fd, buf, count) == -1) {
            print_error("write", write_file_error_msg);
            sys_exit(1);
        }
    }

    sys_close(src_fd);
    sys_close(dst_fd);

    return 0;
}
