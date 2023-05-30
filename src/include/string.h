//
// Created by grsan on 5/22/23.
//

#ifndef GOOSE_STRING_H
#define GOOSE_STRING_H

#include <stddef.h>
#include <stdbool.h>

size_t strlen(const char* str);
void* memset(void* restrict ptr, uint8_t val, size_t size);
void* memcpy(void* restrict dest, const void* restrict src, size_t size);

char* itoa(uint64_t num, char *buf, uint8_t base);
void reverse_str(char *str, int len);

#endif //GOOSE_STRING_H
