#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void print_dir_file_recursive(const char *dirname, const char *filename)
{
    DIR *dir = opendir(dirname);
    if (!dir) {
        perror(dirname);
        exit(EXIT_FAILURE);
    }

    struct dirent *dent;

    while ((dent = readdir(dir)) != NULL) {
        if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, "..")) {
            continue;
        }

        char *path = malloc(strlen(dirname) + strlen(dent->d_name) + 1);
        strcpy(path, dirname);
        strcat(path, "/");
        strcat(path, dent->d_name);

        struct stat path_stat;
        stat(path, &path_stat);

        if (S_ISDIR(path_stat.st_mode)) {
            print_dir_file_recursive(path, filename);
        } else {
            if (!strcmp(dent->d_name, filename)) {
                printf("%s/%s\n", dirname, dent->d_name);
            }
        }

        free(path);
    }

    closedir(dir);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];

    print_dir_file_recursive(".", filename);

    return 0;
}
