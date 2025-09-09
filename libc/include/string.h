//
// Created by Grayson on 9/14/2023.
//

#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

//typedef int size_t;

void* memset(void* restrict ptr, uint8_t val, size_t size);
void* memcpy(void* restrict dest, const void* restrict src, size_t size);

void reverse_str(char *str, int len);

size_t strlen(const char* str);
char *strcpy(char *s, const char *ct);
char *strcat(char *s, const char *ct);
char *strchr(const char *cs, char c);

bool strcmp(const char *a, const char *b);
bool strncmp(const char *a, const char *b, uint32_t l);

#endif //STRING_H
