//
// Created by grsan on 8/14/23.
//

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stddef.h>

#include "../arch/idt.h"

#define CHANNEL_ZERO_DATA 0x40
#define CHANNEL_ONE_DATA 0x41
#define CHANNEL_TWO_DATA 0x42
#define COMMAND_REG 0x43

#define CHANNEL_ZERO_SELECT 0b00000000
#define CHANNEL_ONE_SELECT 0b01000000
#define CHANNEL_TWO_SELECT 0b10000000
#define READ_BACK_COMMAND 0b11000000

#define LATCH_COUNT_VALUE_COMMAND 0b00000000
#define ACCESS_MODE_LOW_BYTE 0b00010000
#define ACCESS_MODE_HIGH_BYTE 0b00100000
#define ACCESS_MODE_LOW_HIGH_BYTE 0b00110000

#define MODE_ZERO 0b00000000
#define MODE_ONE 0b00000010
#define MODE_TWO 0b00000100
#define MODE_THREE 0b00000110
#define MODE_FOUR 0b00001000
#define MODE_FIVE 0b00000000

#define BCD_MODE 0b00000001

void set_timer_phase(uint32_t hz);
void timer_handler();

uint64_t get_millis();

void setup_timer();

#endif //TIMER_H
