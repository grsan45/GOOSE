#include <stdio.h>
#include <unistd.h>

int printf(const char *format, ...) {
    write(1, (char *) format);
    return 0;
}

int fprintf(FILE *f, const char *format, ...) {
    write(0, (char *)format);
    return 0;
}