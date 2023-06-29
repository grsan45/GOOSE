//
// Created by grsan on 5/22/23.
//

#include "../include/display/framebuffer.h"

framebuffer_info fbinfo;
void* vram;

void init_framebuffer(framebuffer_info info) {
    fbinfo = info;
    vram = (uint8_t *) fbinfo.addr;
    memset((uint8_t *) vram, 0x00, info.width * info.height * 3);
}

void put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    uint32_t* pixel = vram + fbinfo.pitch * y + 3 * x;
    *pixel = (color & 0xFFFFFF) | (*pixel & 0xFF000000);
}