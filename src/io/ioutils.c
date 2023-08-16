//
// Created by grsan on 8/14/23.
//

#include "../include/io/ioutils.h"

void outb(uint16_t port, uint8_t data) {
    __asm__ __volatile__ ("out %1, %0" : : "dN" (port), "a" (data));
}

uint8_t inb(uint16_t port) {
    uint8_t recv;
    __asm__ __volatile__ ("in %1, %0" : "=a" (recv) : "dN" (port));
    return recv;
}