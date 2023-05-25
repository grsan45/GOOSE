//
// Created by grsan on 5/22/23.
//

#include <stdint.h>
#include "include/string.h"

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len++]);
    return len-1;
}

void* memset(void* ptr, uint8_t val, size_t size) {
    unsigned char *buf = (unsigned char *)ptr;
    for (size_t i = 0; i < size; i++)
        buf[i] = val;

    return ptr;
}

void* memcpy(void *restrict dest, const void* restrict src, size_t size) {
    unsigned char *destBuf = (unsigned char *) dest;
    const unsigned char *srcBuf = (const unsigned char*) src;
    for (size_t i = 0; i < size; i++)
        destBuf[i] = srcBuf[i];
    return destBuf;
}