//
// Created by grsan on 5/29/23.
//

#ifndef GOOSE_SERIAL_H
#define GOOSE_SERIAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include "ioutils.h"

#define COM1 0x3f8

bool init_serial(uint16_t port);

uint32_t serial_rx(uint16_t port);
uint8_t serial_read(uint16_t port);

uint32_t is_tx_empty(uint16_t port);
void serial_putc(uint16_t port, uint8_t ch);
void serial_puts(uint16_t port, const char* str);
void serial_printf(uint16_t port, const char* format, ...);

#endif //GOOSE_SERIAL_H
