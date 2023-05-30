//
// Created by grsan on 5/30/23.
//

#include "../include/io/serial.h"
#include "../include/string.h"

// mostly stolen from https://wiki.osdev.org/Serial_Ports
bool init_serial(uint16_t port) {
    outb(port + 1, 0x00);    // Disable all interrupts
    outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(port + 1, 0x00);    //                  (hi byte)
    outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(port + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(port + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    if (inb(port + 0) != 0xAE)
        return true;

    outb(port + 4, 0x0F);
    return false;
}

uint32_t serial_rx(uint16_t port) {
    return inb(port + 5) & 1;
}

uint8_t serial_read(uint16_t port) {
    while (serial_rx(port) == 0);

    return inb(port);
}

uint32_t is_tx_empty(uint16_t port) {
    return inb(port + 5) & 0x20;
}

void serial_putc(uint16_t port, uint8_t ch) {
    while (is_tx_empty(port) == 0);

    outb(port, ch);
}

// his ass is NOT formatting
void serial_printf(uint16_t port, const char* str) {
    uint32_t len = strlen(str);
    for (uint32_t i = 0; i < len; i++)
        serial_putc(port, str[i]);
}