//
// Created by grsan on 5/29/23.
//

#ifndef GOOSE_UTILS_H
#define GOOSE_UTILS_H

#include <stdint.h>

void outb(uint16_t port, uint8_t data) {
    __asm__ __volatile__ ("out %1, %0" : : "dN" (port), "a" (data));
}

uint8_t inb(uint16_t port) {
    uint8_t recv;
    __asm__ __volatile__ ("in %1, %0" : "=a" (recv) : "dN" (port));
    return recv;
}

#endif //GOOSE_UTILS_H
