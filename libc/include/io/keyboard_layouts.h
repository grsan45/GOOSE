//
// Created by grsan on 8/17/23.
//

#ifndef KEYBOARD_LAYOUTS_H
#define KEYBOARD_LAYOUTS_H

#include <stdint.h>

#define SHIFT_KEY 0b10000001
#define ALT_KEY 0b10000010
#define CTRL_KEY 0b10000100
#define CAPSLOCK_KEY 0b10001000
#define NUMLOCK_KEY 0b10010000

// http://www.osdever.net/bkerndev/Docs/keyboard.htm
static uint8_t kbdus[128] = {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
        '9', '0', '-', '=', '\b',    /* Backspace */
        '\t',            /* Tab */
        'q', 'w', 'e', 'r',    /* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',    /* Enter key */
        CTRL_KEY,            /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',    /* 39 */
        '\'', '`', SHIFT_KEY,        /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
        'm', ',', '.', '/', SHIFT_KEY,                /* Right shift */
        '*',
        ALT_KEY,    /* Alt */
        ' ',    /* Space bar */
        CAPSLOCK_KEY,    /* Caps lock */
        0,    /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0,
        0,    /* < ... F10 */
        NUMLOCK_KEY,    /* 69 - Num lock*/
        0,    /* Scroll Lock */
        0,    /* Home key */
        0,    /* Up Arrow */
        0,    /* Page Up */
        '-',
        0,    /* Left Arrow */
        0,
        0,    /* Right Arrow */
        '+',
        0,    /* 79 - End key*/
        0,    /* Down Arrow */
        0,    /* Page Down */
        0,    /* Insert Key */
        0,    /* Delete Key */
        0, 0, 0,
        0,    /* F11 Key */
        0,    /* F12 Key */
        0,    /* All other keys are undefined */
};

static uint8_t kbdus_shifted[128] = {
        0, 27, '!', '@', '#', '$', '%', '^', '&', '*',    /* 9 */
        '(', ')', '_', '+', '\b',    /* Backspace */
        '\t',            /* Tab */
        'Q', 'W', 'E', 'R',    /* 19 */
        'T', 'T', 'U', 'I', 'O', 'P', '{', '}', '\n',    /* Enter key */
        CTRL_KEY,            /* 29   - Control */
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',    /* 39 */
        '"', '~', SHIFT_KEY,        /* Left shift */
        '|', 'Z', 'X', 'C', 'V', 'B', 'N',            /* 49 */
        'M', '<', '>', '?', SHIFT_KEY,                /* Right shift */
        '*', //numpad mul
        ALT_KEY,    /* Alt */
        ' ',    /* Space bar */
        CAPSLOCK_KEY,    /* Caps lock */
        0,    /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0,
        0,    /* < ... F10 */
        NUMLOCK_KEY,    /* 69 - Num lock*/
        0,    /* Scroll Lock */
        0,    /* Home key */
        0,    /* Up Arrow */
        0,    /* Page Up */
        '-', //numpad sub
        0,    /* Left Arrow */
        0,
        0,    /* Right Arrow */
        '+', //numpad add
        0,    /* 79 - End key*/
        0,    /* Down Arrow */
        0,    /* Page Down */
        0,    /* Insert Key */
        0,    /* Delete Key */
        0, 0, 0,
        0,    /* F11 Key */
        0,    /* F12 Key */
        0,    /* All other keys are undefined */
};

#endif //KEYBOARD_LAYOUTS_H
