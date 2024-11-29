#include <stdio.h>
#include <sys/syscall.h>

int printf(const char *format, ...) {
    syscall(SYS_WRITE, 0, format);
    return 0;
}

int fprintf(FILE *f, const char *format, ...) {
    syscall(SYS_WRITE, 1, format);
    return 0;
}