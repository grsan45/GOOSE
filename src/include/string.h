//
// Created by grsan on 5/22/23.
//

#ifndef GOOSE_STRING_H
#define GOOSE_STRING_H

#include <stddef.h>

size_t strlen(const char* str);
void* memset(void* restrict ptr, uint8_t val, size_t size);
void* memcpy(void* restrict dest, const void* restrict src, size_t size);

#endif //GOOSE_STRING_H
