//
// Created by grsan on 8/15/23.
//

#include "../include/io/keyboard.h"
#include "../include/io/keyboard_layouts.h"
#include "../include/io/serial.h"
#include "../include/display/framebuffer.h"

uint8_t modifiers = 0;
uint8_t *kb_layout = kbdus;

// this entire method is hideous, my apologies.
void keyboard_handler() {
    uint8_t scancode;
    scancode = inb(KEYBOARD_PORT);

    if (scancode & 0x80) { // release
        scancode &= ~0x80;
        switch (kb_layout[scancode]) {
            case SHIFT_KEY:
                modifiers &= ~SHIFT;
                break;
            case CAPSLOCK_KEY:
                if (modifiers & UNLOCK) {
                    modifiers &= ~CAPSLOCK;
                    modifiers &= ~UNLOCK;
                }
                break;
            case ALT_KEY:
                modifiers &= ~ALT;
                break;
            case CTRL_KEY:
                modifiers &= ~CTRL;
                break;
            case NUMLOCK_KEY:
                if (modifiers & UNLOCK) {
                    modifiers &= ~NUMLOCK;
                    modifiers &= ~UNLOCK;
                }
                break;
        }
    } else { //press
        switch (kb_layout[scancode]) {
            case SHIFT_KEY:
                modifiers |= SHIFT;
                break;
            case CAPSLOCK_KEY:
                if (modifiers & CAPSLOCK)
                    modifiers |= UNLOCK;
                modifiers |= CAPSLOCK;
                break;
            case ALT_KEY:
                modifiers |= ~ALT;
                break;
            case CTRL_KEY:
                modifiers |= ~CTRL;
                break;
            case NUMLOCK_KEY:
                if (modifiers & NUMLOCK)
                    modifiers |= UNLOCK;
                modifiers |= ~NUMLOCK;
                break;
            default:
                if (modifiers & CAPSLOCK) {
                    kb_layout = kbdus_shifted;
                } else {
                    kb_layout = kbdus;
                }
                if (modifiers & SHIFT) {
                    if (modifiers & CAPSLOCK) {
                        kb_layout = kbdus;
                    }
                    else
                        kb_layout = kbdus_shifted;
                }
                putc(kb_layout[scancode]);
        }
    }
}

void setup_keyboard() {
    set_irq_handler(1, keyboard_handler);
}