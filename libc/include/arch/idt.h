//
// Created by grsan on 5/25/23.
//

#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct idt_entry {
    uint16_t offset_low;
    uint16_t seg_select;
    uint8_t zero;
    uint8_t flags;
    uint16_t offset_high;
} __attribute__((packed)) idt_entry;

typedef struct idt_ptr {
    uint16_t size;
    uint32_t offset;
} __attribute__((packed)) idt_ptr;

typedef enum gate_type {
    TASK=0b0101,
    INT_16=0b0110,
    TRAP_16=0b0111,
    INT_32=0b1110,
    TRAP_32=0b1111,
} gate_type;

extern void idt_load();

void add_idt_entry(uint8_t num, uint32_t offset, uint16_t segment, gate_type gate_type, uint8_t ring);
void idt_install();

typedef struct {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_id, error_code;
    uint32_t eip, cs, eflags, useresp, ss; //pushed automatically by the processor
} isr_stacktrace;

void set_irq_handler(uint8_t irq, void (*handler)(isr_stacktrace *r));

void install_isrs();
void install_irqs();

void handle_fault(isr_stacktrace *r);
void handle_irq(isr_stacktrace *r);

#endif //IDT_H
