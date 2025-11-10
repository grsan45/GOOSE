#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

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
        memcpy(buf, "0", 2);
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

uint32_t itoa(int32_t num, char *buf) {
    return itob(num, buf, 10);
}

int32_t atob(char* str, uint8_t base) {
    bool negative = str[0] == '-';
    if (negative) str++;

    int32_t ret = 0;
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

int32_t atoi(char* str) {
    return atob(str, 10);
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

bool strcmp(const char *a, const char *b) {
    uint32_t a_len = strlen(a);
    uint32_t b_len = strlen(b);
    if (a_len != b_len) return false;
    return strncmp(a, b, a_len > b_len ? b_len : a_len);
}

int sprintf(char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    return vsnprintf(UINT32_MAX, str, format, args);
}

int snprintf(size_t size, char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    return vsnprintf(size, str, format, args);
}

int vsnprintf(size_t size, char *str, const char *format, va_list args) {
    char *tmp_copy = 0;
    uint32_t tmp_copy_len = 0;

    int32_t flag_start = -1;

    uint32_t out_idx = 0;
    uint32_t fmt_len = strlen(format);
    for (uint32_t i = 0; i < fmt_len; i++) {
        if (out_idx >= size - 1) return 2; // todo: return # of bytes that would have been written

        if ((format[i] == '%' || flag_start > 0) && format[++i]) {
            flag_start = i;
            switch (format[i]) {
                case '%':
                    str[out_idx++] = '%';

                    flag_start = -1;
                    continue;
                case 's':
                    tmp_copy = va_arg(args, char*);
                    tmp_copy_len = strlen(tmp_copy);
                    memcpy(str + out_idx, tmp_copy, tmp_copy_len);
                    out_idx += tmp_copy_len;

                    flag_start = -1;
                    continue;
                case 'd':
                    // todo: flag support
                    tmp_copy_len = itob(va_arg(args, uint32_t), str + out_idx, 10);
                    out_idx += tmp_copy_len;

                    flag_start = -1;
                    continue;
                case 'x':
                    memcpy(str + out_idx, "0x", 2);
                    out_idx += 2;

                    tmp_copy_len = itob(va_arg(args, uint32_t), str + out_idx, 16);
                    out_idx += tmp_copy_len;

                    flag_start = -1;
                    continue;
                default:
                    continue;
            }
        }

        str[out_idx++] = format[i];
    }

    str[out_idx] = '\0';

    if (flag_start > 0) return -1; // never found a directive, invalid format

    return 0;
}

