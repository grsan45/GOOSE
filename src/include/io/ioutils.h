//
// Created by grsan on 5/29/23.
//

#ifndef GOOSE_IOUTILS_H
#define GOOSE_IOUTILS_H

#include <stdint.h>

void outb(uint16_t port, uint8_t data);

uint8_t inb(uint16_t port);

#endif //GOOSE_IOUTILS_H
