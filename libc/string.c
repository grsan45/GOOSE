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

uint32_t itob(int32_t num, char* buf, uint8_t base) {
    uint8_t i = 0;

    if (num == 0) {
        buf = "0";
        return 1;
    }

    bool negative = num < 0;
    if (negative) num = -num;

    while (num != 0) {
        uint8_t rem = num % base;
        buf[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (negative) buf[i++] = '-';

    reverse_str(buf, i);
    buf[i] = '\0';

    return i;
}

uint32_t atob(char* str, uint8_t base) {
    bool negative = str[0] == '-';
    if (negative) str++;

    uint32_t ret = 0;
    char digit;
    while((digit = *(str++)) != '\0') {
        ret *= base;

        int digit_offset = '0';
        if (digit >= 'a') digit_offset = 'a' - 10;
        else if (digit >= 'A') digit_offset = 'A' - 10;
        ret += digit - digit_offset;
    }
    return ret * (negative ? -1 : 1);
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