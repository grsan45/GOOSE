//
// Created by grsan on 5/22/23.
//

#include <display/framebuffer.h>
#include <stdlib.h>

framebuffer_info fbinfo;
void* vram;

uint32_t cursor_x;
uint32_t cursor_y;

uint32_t fg = 0xFFFFFF;
uint32_t bg = 0x000000;

void init_framebuffer(framebuffer_info info) {
    fbinfo = info;
    vram = (uint8_t *) fbinfo.addr;
    memset((uint8_t *) vram, 0x00, info.width * info.height * 3);
}

void clear() {
    memset((uint8_t *) vram, 0x00, fbinfo.width * fbinfo.height * 3);
}

void clearLine() {
    for (uint32_t y = cursor_y * font->height; y < cursor_y * font->height + font->height; y++) {
        memset((uint8_t *) vram + (y * fbinfo.width), 0x00, fbinfo.width * 3);
    }
}

void put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    uint32_t* pixel = vram + fbinfo.pitch * y + 3 * x;
    *pixel = (color & 0xFFFFFF) | (*pixel & 0xFF000000);
}

void set_cursor(uint32_t x, uint32_t y) {
    cursor_x = x;
    cursor_y = y;
}

void set_color(uint32_t new_fg, uint32_t new_bg) {
    fg = new_fg;
    bg = new_bg;
}

void increment_cursor() {
    cursor_x++;
    if (cursor_x > fbinfo.width / font->width) {
        cursor_x = 0;
        cursor_y++;
    }

    // slow operation to move the entire framebuffer up if we reach the bottom
    // will not be an issue once we go to long mode and have faster memory operations
    if (cursor_y > fbinfo.height / font->height - 1) {
        uint32_t row_size = fbinfo.pitch * font->height;
        memcpy(fbinfo.addr, fbinfo.addr + row_size, (fbinfo.height * fbinfo.pitch) - row_size);
        cursor_y--;
    }
}

void putc(uint8_t ch) {
    if (ch == '\n') {
        cursor_x = 0;
        cursor_y++;
        return;
    }
    if (ch == '\r') {
        cursor_x = 0;
        return;
    }
    if (ch == '\b') {
        if (cursor_x == 0) {
            cursor_y--;
            cursor_x = fbinfo.width / font->width;
        } else {
            cursor_x--;
        }

        uint32_t x = cursor_x * font->width;
        uint32_t y = cursor_y * font->height;
        for (uint8_t row = 0; row < font->height; row++) {
            for (uint8_t col = 0; col < font->width; col++) {
                put_pixel(x + col, y + row, bg);
            }
        }
        return;
    }

    glyph g = get_glyph((uint16_t) ch);

    uint32_t x = cursor_x * font->width;
    uint32_t y = cursor_y * font->height;

    for (uint8_t row = 0; row < font->height; row++) {
        for (uint8_t col = 0; col < font->width; col++) {
            uint8_t glyph_row = reverse_byte(g[row]); // big endian moment
            put_pixel(x + col, y + row, glyph_row & (1 << col) ? fg : bg);
        }
    }

    increment_cursor();
}

void puts(const char* str) {
    for (uint32_t i = 0; i < strlen(str); i++) {
        putc(str[i]);
    }
}

void fb_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    uint8_t base = 0;

    uint32_t len = strlen(format);
    char *digitbuffer = malloc(65);
    for (uint32_t i = 0; i < len; i++) {
        if (format[i] == '%' && format[++i]) {
            insideloop:
            switch (format[i]) {
                case 's':
                    puts(va_arg(args, char*));
                    break;
                case '%':
                    putc('%');
                    break;
                case 'd':
                    puts(itob(va_arg(args, uint32_t), digitbuffer, base > 1 ? base : 10));
                    base = 0;
                    break;
                default:
                    if (format[i] >= '0' && format[i] <= '9') {
                        base = 0;
                        while (format[i+1] && format[i] != 'd') {
                            base *= 10;
                            base += format[i] - '0';
                            i++;
                        }
                        goto insideloop;
                    }
                    break;
            }
            i++;
        }

        putc(format[i]);
    }

    free(digitbuffer);

    va_end(args);
}