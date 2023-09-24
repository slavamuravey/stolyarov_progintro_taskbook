#ifndef SYSCALL_H_SENTRY
#define SYSCALL_H_SENTRY

extern int sys_errno;
int sys_exit(int code);
int sys_read(int fd, const void *buf, unsigned int size);
int sys_write(int fd, const void *buf, unsigned int size);
int sys_open(const char *filename, unsigned int len, ...);
int sys_close(int fd);

#endif