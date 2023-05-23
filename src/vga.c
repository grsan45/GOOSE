//
// Created by grsan on 5/22/23.
//

#include "include/vga.h"

static size_t term_row;
static size_t term_col;
static uint8_t term_color;
static uint16_t* term_buffer;

void init_vga_terminal() {
    term_row = 0;
    term_col = 0;
    set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    term_buffer = (uint16_t*)VGA_ADDR;

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++)
            term_buffer[y * VGA_WIDTH + x] = (uint16_t) ' ' | (uint16_t) term_color << 8;
    }
}

void set_color(vga_color fg, vga_color bg) {
    term_color = fg | bg << 4;
}

void vga_putc(const char c) {
    if (c == '\n') {
        term_col = 0;
        if (++term_row == VGA_HEIGHT) {
            term_row--;
            memcpy(term_buffer, term_buffer + VGA_WIDTH, VGA_WIDTH * (VGA_HEIGHT - 1) * 4); // move up 1 line
        }
        return;
    }

    term_buffer[term_row * VGA_WIDTH + term_col] = c | term_color << 8;
    if (++term_col == VGA_WIDTH) {
        term_col = 0;
        if (++term_row == VGA_HEIGHT) {
            term_row--;
            memcpy(term_buffer, term_buffer + VGA_WIDTH * 2, VGA_WIDTH * (VGA_HEIGHT - 1) * 4); // move up 1 line
        }
    }
}

void vga_print(const char *str) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
        vga_putc(str[i]);
}