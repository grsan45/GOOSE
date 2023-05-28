//
// Created by grsan on 5/22/23.
//

#ifndef GOOSE_FRAMEBUFFER_H
#define GOOSE_FRAMEBUFFER_H

#include <stddef.h>
#include <stdint.h>
#include "../string.h"

typedef struct {
    uint64_t addr;
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
void putc(unsigned char ch, uint8_t x, uint8_t y);

#endif //GOOSE_FRAMEBUFFER_H
