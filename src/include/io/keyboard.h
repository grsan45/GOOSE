//
// Created by grsan on 8/15/23.
//

#ifndef GOOSE_KEYBOARD_H
#define GOOSE_KEYBOARD_H

#include <stdint.h>
#include "../arch/idt.h"
#include "../display/framebuffer.h"
#include "ioutils.h"

#define KEYBOARD_PORT 0x60

#define SHIFT 0b00000001
#define ALT 0b00000010
#define CTRL 0b00000100
#define CAPSLOCK 0b00001000
#define NUMLOCK 0b00010000
#define UNLOCK 0b1000000

void keyboard_handler();
void setup_keyboard();

#endif //GOOSE_KEYBOARD_H
