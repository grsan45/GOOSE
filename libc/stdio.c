#include <stdio.h>
#include <unistd.h>

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char *buf = malloc(sizeof(char) * 128);
    vsnprintf(128, buf, format, args);
    va_end(args);

    write(1, buf);
    free(buf);
    return 0;
}

int fprintf(FILE *f, const char *format, ...) {
    write(0, (char *)format);
    return 0;
}

FILE *fopen(const char *filename, const char *mode) {
    if (true == strncmp("klog", filename, 4)) { // temp manual check for klog
        FILE *ret = malloc(sizeof(FILE));

        ret->descriptor = 1;
        ret->start = malloc(sizeof(char) * FILE_BUFFER_SIZE);
        ret->cursor = ret->start;
        ret->end = ret->start + FILE_BUFFER_SIZE - 1;

        return ret;
    }

    return 0;
}
