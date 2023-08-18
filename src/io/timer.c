//
// Created by grsan on 8/14/23.
//

#include "../include/io/timer.h"
#include "../include/io/ioutils.h"

static uint64_t millis_since_startup = 0;

void set_timer_phase(uint32_t hz) {
    uint16_t divisor = (7159090 + 6/2) / (6 * hz);
    outb(COMMAND_REG, ACCESS_MODE_LOW_HIGH_BYTE | MODE_THREE);
    outb(CHANNEL_ZERO_DATA, divisor & 0xFF);
    outb(CHANNEL_ZERO_DATA, (divisor & 0xff00) >> 8);
}

void timer_handler() {
    millis_since_startup++;
}

uint64_t get_millis() {
    return millis_since_startup;
}

void setup_timer() {
    set_timer_phase(1000);
    set_irq_handler(0, timer_handler);
}