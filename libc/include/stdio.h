#ifndef GOOSE_STDIO_H
#define GOOSE_STDIO_H

#include <stdarg.h>
#include "string.h"

typedef struct {
    uint32_t descriptor;
    char* start, end, cursor;
} FILE;
extern FILE* stderr;
extern FILE* stdout;

#define stdout stdout
#define stderr stderr

FILE *fopen(const char *filename, const char *mode);
size_t fread(void *ptr, size_t size, size_t num_obj, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t num_obj, FILE *stream);

int fclose(FILE *stream);
void fflush(FILE *stream);

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
void fseek(FILE *stream, long offset, int origin);
long ftell(FILE *stream);

int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);
int sprintf(char *s, const char *format, ...);
int vfprintf(FILE *stream, const char *format, va_list arg);

void setbuf(FILE *stream, char *buf);

#endif //GOOSE_STDIO_H
