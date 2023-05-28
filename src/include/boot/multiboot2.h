//
// Created by grsan on 5/26/23.
//

#ifndef GOOSE_MULTIBOOT2_H
#define GOOSE_MULTIBOOT2_H

#include <stdint.h>

typedef struct {
    uint16_t type;
    uint16_t flags;
    uint32_t size;
} multiboot_tag;

//TODO: implement the remaining tags later, only really care about this one right now.
typedef struct {
    uint16_t type;
    uint16_t flags;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
    uint16_t reserved;
} multiboot_framebuffer_tag_common;

typedef struct {
    multiboot_framebuffer_tag_common common;
    union {
        struct {
            uint8_t red_location;
            uint8_t red_bit_depth;
            uint8_t green_location;
            uint8_t green_bit_depth;
            uint8_t blue_location;
            uint8_t blue_bit_depth;
        };
    };
} multiboot_framebuffer_tag;

#endif //GOOSE_MULTIBOOT2_H
