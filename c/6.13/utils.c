#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include "dynamic_array.h"

static bool is_space_char(char c)
{
    return c == ' ' || c == '\t';
}

bool match_cmd(const char *str, const char *cmd)
{
    const char *start_ptr, *end_ptr;
    size_t cmd_len, actual_cmd_len;
    for (start_ptr = str; is_space_char(*start_ptr); start_ptr++) {}
    for (end_ptr = str + strlen(str) - 1; is_space_char(*end_ptr); end_ptr--) {}
    cmd_len = strlen(cmd);
    actual_cmd_len = end_ptr - start_ptr + 1;
    if (actual_cmd_len != cmd_len) {
        return false;
    }

    if (strncmp(start_ptr, cmd, cmd_len)) {
        return false;
    }

    return true;
}

int extract_cmd(struct dynamic_array *buf, char *cmd)
{
    int i;
    for (i = 0; i < buf->len; i++) {
        char *bytes = buf->ptr;
        if (bytes[i] == '\n') {
            memcpy(cmd, bytes, i);
            cmd[i] = '\0';
            if (cmd[i - 1] == '\r') {
                cmd[i - 1] = '\0';
            }
            dynamic_array_slice(buf, i, buf->len - 1);

            return 0;
        }
    }

    return 1;
}