//
// Created by grsan on 5/26/23.
//

#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#include <stdint.h>

typedef struct {
    uint16_t type;
    uint16_t flags;
    uint32_t size;
} multiboot_tag_t;

//TODO: implement the remaining tags later
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
} multiboot_framebuffer_tag_common_t;

typedef struct {
    multiboot_framebuffer_tag_common_t common;
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
} multiboot_framebuffer_tag_t;

typedef struct {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} multiboot_memory_map_entry_t;

typedef struct {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version; // always 0
    multiboot_memory_map_entry_t entries[];
} multiboot_memory_map_t;

#endif //MULTIBOOT2_H
