#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define WAKEUP_TIMEOUT 5

volatile static sig_atomic_t sigusr1_count = 0;
volatile static sig_atomic_t start_time;

void write_daemon_info()
{
    char str[500];
    char filename[100];
    sprintf(
        str, 
        "I am SAV daemon :) Pid: %d, uptime, sec.: %ld, handled SUGUSR1 count: %d\n", 
        getpid(), 
        time(NULL) - start_time, 
        sigusr1_count
    );
    sprintf(filename, "/tmp/sav_daemon.%d.log", start_time);
    int fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
    int save_errno = errno;
    write(fd, str, strlen(str));
    errno = save_errno;
    close(fd);
}

void sigusr1_handler(int s) {
    signal(SIGUSR1, sigusr1_handler);
    sigusr1_count++;

    write_daemon_info();
}

void sigalrm_handler(int s) {
    signal(SIGALRM, sigalrm_handler);
    alarm(WAKEUP_TIMEOUT);

    write_daemon_info();
}

int main(int argc, char **argv)
{
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_WRONLY);
    chdir("/");

    pid_t pid;
    
    /* fork process to change pid before setsid */
    pid = fork();
    if (pid == -1) {
        perror("fork");
    }

    if (pid > 0) {
        exit(0);
    }

    setsid();

    /* fork process to stop be session leader */
    pid = fork();
    if (pid == -1) {
        perror("fork");
    }

    if (pid > 0) {
        exit(0);
    }

    start_time = time(NULL);

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGALRM, sigalrm_handler);
    alarm(WAKEUP_TIMEOUT);

    while (1) {
        pause();
    }

    return 0;
}
