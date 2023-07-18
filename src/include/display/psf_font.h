//
// Created by grsan on 7/17/23.
//

#ifndef GOOSE_PSF_FONT_H
#define GOOSE_PSF_FONT_H

#include <stdint.h>
#include <stddef.h>

#define PSF2_MAGIC_NUMBER 0x864ab572

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t header_size;
    uint32_t flags;
    uint32_t length;
    uint32_t glyph_size;
    uint32_t height;
    uint32_t width;
} PSF2_font;

typedef uint8_t* glyph;

extern PSF2_font *font;

void init_psf();
glyph get_glyph(uint16_t c);

#endif //GOOSE_PSF_FONT_H
