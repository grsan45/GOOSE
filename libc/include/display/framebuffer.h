//
// Created by grsan on 5/22/23.
//

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "../string.h"
#include "psf_font.h"
#include "../math.h"
#include "../arch/memmgt.h"

typedef struct {
    void *addr;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t depth;

    uint8_t red_location;
    uint8_t red_bit_depth;
    uint8_t green_location;
    uint8_t green_bit_depth;
    uint8_t blue_location;
    uint8_t blue_bit_depth;
} framebuffer_info;

void init_framebuffer(framebuffer_info info);
void put_pixel(uint32_t x, uint32_t y, uint32_t color);

void clear();
void clearLine();

void set_cursor(uint32_t x, uint32_t y);
void increment_cursor();

void set_color(uint32_t fg, uint32_t bg);

void putc(uint8_t ch);
void puts(const char* str);
void printf(const char *format, ...);

#endif //FRAMEBUFFER_H
