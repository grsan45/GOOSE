//
// Created by grsan on 5/23/23.
//

#ifndef GOOSE_GDT_H
#define GOOSE_GDT_H

#include <stdint.h>

typedef struct gdt_entry {
    uint16_t limit_low; // bits 0-15
    uint16_t base_low; // bits 0-15
    uint8_t base_mid; // bits 16-23
    uint8_t access;
    uint8_t flags; // granularity (high) and limit (low, bits 16-19)
    uint8_t base_high; // bits 23-31
} __attribute__((packed)) gdt_entry;

typedef struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr;

extern void gdt_flush();

void gdt_set_entry(uint8_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);
void gdt_install();

#endif //GOOSE_GDT_H
