//
// Created by grsan on 7/17/23.
//

#include "../include/display/psf_font.h"
#include "../include/arch/memmgt.h"
#include "../include/io/serial.h"

// NOTE: obsolete once FS in implemented
extern PSF2_font _binary_Tamsyn8x16b_psf_start;
extern uint8_t _binary_Tamsyn8x16b_psf_end;

uint16_t *unicode;

PSF2_font *font;

void init_psf() {
//    uint16_t *glyph;
    font = &_binary_Tamsyn8x16b_psf_start; // big pointer enjoyer

    if (font->magic != PSF2_MAGIC_NUMBER) {
        serial_printf(COM1, "Invalid font format.\n");
        return;
    }

    if (font->flags) {
        serial_printf(COM1, "No unicode table present.\n");
        unicode = NULL;
        return;
    }

    // TODO: once I find a font with a unicode table, I'll implement this.
    uint8_t *table = (uint8_t *) (&_binary_Tamsyn8x16b_psf_start + font->header_size + font->length * font->glyph_size);
    unicode = calloc(UINT16_MAX, 2);

    while (table > &_binary_Tamsyn8x16b_psf_end) {

    }
}

glyph get_glyph(uint16_t c) {
    if (unicode != NULL)
        c = unicode[c];

    return (uint8_t*) &_binary_Tamsyn8x16b_psf_start + font->header_size + c * font->glyph_size;
}