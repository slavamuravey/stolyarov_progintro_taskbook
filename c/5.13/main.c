#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/wait.h>

struct process_info {
    pid_t pid;
    const char *cmd;
    struct process_info *next;
};

struct process_info *get_process_info_by_pid(struct process_info *first, pid_t pid)
{
    struct process_info *tmp;
    for (tmp = first; tmp; tmp = tmp->next) {
        if (tmp->pid == pid) {
            return tmp;
        }
    }

    return NULL;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments.\n");
        exit(1);
    }

    struct process_info *first = NULL, *tmp;

    char *argvs[argc];

    for (int i = 1; i < argc; i++) {
        argvs[i - 1] = strcmp(argv[i], ";;") ? argv[i] : NULL;
    }
    argvs[argc - 1] = NULL;

    bool within_cmd = false;

    for (int i = 0; i < argc; i++) {
        if (argvs[i] != NULL && within_cmd == false) {
            within_cmd = true;

            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                exit(1);
            }            

            const char *file = argvs[i];
            tmp = malloc(sizeof(struct process_info));
            tmp->pid = pid;
            tmp->cmd = file;
            tmp->next = first;
            first = tmp;

            if (pid == 0) {
                execvp(file, argvs + i);
                perror(file);
                exit(1);
            }

            continue;
        }

        if (argvs[i] == NULL) {
            within_cmd = false;
        }
    }    

    for (tmp = first; tmp; tmp = tmp->next) {
        int status;
        pid_t pid = wait3(&status, 0, NULL);

        if (WIFEXITED(status)) {
            if (status == 0) {
                struct process_info *process_info = get_process_info_by_pid(first, pid);
                printf("%s\n", process_info->cmd);
            }
        }
    }

    while (first) {
        tmp = first;
        first = first->next;
        free(tmp);
    }

    return 0;
}
