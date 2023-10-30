#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include "arch/memmgt.h"

void abort();

int atexit(void  (*f)(void));
void exit(int status);

char *getenv(const char *name);

uint64_t atoi(char* str);
char *itob(uint64_t num, char *buf, uint8_t base);

int abs(int i);

#endif //STDLIB_H
