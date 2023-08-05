#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

int main(int argc, char **argv)
{
    struct termios oflags, nflags;

    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    if (!isatty(STDIN_FILENO)) {
        exit(0);
    }

    char *filename = argv[1];
    FILE *stream = fopen(filename, "r");
    if (!stream) {
        perror(filename);
        exit(1);
    }

    char password_part[4];
    size_t password_part_cap = sizeof(password_part);
    size_t password_cap = password_part_cap;
    size_t password_len = 0;
    char *password = calloc(password_cap, sizeof(*password_part));
    while (fgets(password_part, password_part_cap, stream)) {
        password_len = strlen(password);
        size_t password_part_len = strlen(password_part);
        size_t new_password_len = password_len + password_part_len;
        if (new_password_len > password_cap - 1) {
            password_cap *= 2;
            char *tmp_password = malloc(password_cap);
            memcpy(tmp_password, password, password_len);
            free(password);
            password = tmp_password;
        }
        memcpy(password + password_len, password_part, password_part_len);
        password[new_password_len] = '\0';
    }
    password_len = strlen(password);

    char *last_password_char_ptr = password + password_len - 1;
    if (*last_password_char_ptr == '\n') {
        *last_password_char_ptr = '\0';
    }
    
    printf("password: ");

    tcgetattr(STDIN_FILENO, &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    if (tcsetattr(STDIN_FILENO, TCSADRAIN, &nflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }

    size_t entered_password_cap = 4;
    size_t entered_password_len = 0;
    char *entered_password = calloc(entered_password_cap, sizeof(char));
    int exit_status = 0;
    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            if (strcmp(entered_password, password)) {
                fputs("incorrect password", stderr);
                exit_status = 1;
            }

            break;
        }
        entered_password_len++;
        if (entered_password_len > entered_password_cap - 1) {
            entered_password_cap *= 2;
            char *tmp_entered_password = malloc(entered_password_cap);
            memcpy(tmp_entered_password, entered_password, entered_password_len);
            free(entered_password);
            entered_password = tmp_entered_password;
        }

        entered_password[entered_password_len - 1] = c;
        entered_password[entered_password_len] = '\0';
    }

    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        exit(1);
    }

    putchar('\n');

    free(password);
    free(entered_password);

    return exit_status;
}
