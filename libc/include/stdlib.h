#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include "arch/memmgt.h"

void abort();

int atexit(void  (*f)(void));
void exit(int status);

char *getenv(const char *name);

int32_t atob(char* str, uint8_t radix);
int32_t atoi(char* str);

uint32_t itob(int32_t num, char *buf, uint8_t radix);
uint32_t itoa(int32_t num, char *buf);

int abs(int i);

#endif //STDLIB_H
