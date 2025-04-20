#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include "arch/memmgt.h"

void abort();

int atexit(void  (*f)(void));
void exit(int status);

char *getenv(const char *name);

uint32_t atoi(const char *str);
uint32_t atoi_base(const char *str, uint8_t base);
char *itob(uint32_t num, char *buf, uint8_t base);

int abs(int i);

#endif //STDLIB_H
