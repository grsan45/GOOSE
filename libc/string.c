//
// Created by grsan on 5/22/23.
//

#include <stdlib.h>
#include <string.h>

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len++]);
    return len-1;
}

void* memset(void* ptr, uint8_t val, size_t size) {
    uint8_t *buf = (uint8_t *)ptr;
    for (size_t i = 0; i < size; i++)
        buf[i] = val;

    return ptr;
}

void* memcpy(void *restrict dest, const void* restrict src, size_t size) {
    uint8_t *destBuf = (uint8_t *) dest;
    const uint8_t *srcBuf = (const uint8_t *) src;
    for (size_t i = 0; i < size; i++)
        destBuf[i] = srcBuf[i];
    return destBuf;
}

// doesn't support negatives :)
char* itob(uint32_t num, char* buf, uint8_t base) {
    uint8_t i = 0;

    if (num == 0) {
        buf = "0";
        return buf;
    }

    while (num != 0) {
        uint8_t rem = num % base;
        buf[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    reverse_str(buf, i);
    buf[i] = '\0';

    return buf;
}

uint32_t atoi(const char *str) {
    return atoi_base(str, 10);
}

uint32_t atoi_base(const char* str, uint8_t base) {
    uint32_t ret = 0;
    char digit;
    uint32_t place = 1;
    while((digit = *(str++)) != '\0') {
        bool arabic_numeral = digit >= '0' && digit <= '9';
        ret += place * base * (digit - (arabic_numeral ? '0' : 'a'));
        place *= 10;
    }
    return ret;
}

void reverse_str(char *str, int len) {
    int start = 0;
    int end = len - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}
