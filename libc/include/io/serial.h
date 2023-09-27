//
// Created by grsan on 5/29/23.
//

#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#define COM1 0x3f8

bool init_serial(uint16_t port);

uint32_t serial_rx(uint16_t port);
uint8_t serial_read(uint16_t port);

uint32_t is_tx_empty(uint16_t port);
void serial_putc(uint16_t port, uint8_t ch);
void serial_puts(uint16_t port, const char* str);
void serial_printf(uint16_t port, const char* format, ...);

#endif //SERIAL_H
