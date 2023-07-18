//
// Created by grsan on 5/25/23.
//

#include "../include/arch/idt.h"
#include "../include/arch/isrs.h"
#include "../include/string.h"
#include "../include/io/serial.h"
#include "../include/display/framebuffer.h"

idt_entry idt_entries[256];
idt_ptr idtr;

static const char* exception_messages[32] = {
        "Division By Zero Exception.",
        "Debug Exception.",
        "Non Maskable Interrupt Exception.",
        "Breakpoint Exception.",
        "Into Detected Overflow Exception.",
        "Out of Bounds Exception.",
        "Invalid Opcode Exception.",
        "No Coprocessor Exception.",
        "Double fault Exception.",
        "Coprocessor Segment Overrun Exception.",
        "Bad TSS Exception.",
        "Segment Not Present Exception.",
        "Stack Fault Exception.",
        "General Protection Fault Exception.",
        "Page Fault Exception.",
        "Unknown Interrupt Exception.",
        "Coprocessor Fault Exception.",
        "Alignment Check Exception.",
        "Machine Check Exception.",
        "Reserved Interrupt",
        "Reserved Interrupt",
        "Reserved Interrupt",
        "Reserved Interrupt",
        "Reserved Interrupt",
        "Reserved Interrupt",
        "Reserved Interrupt",
        "Reserved Interrupt",
        "Reserved Interrupt",
        "Reserved Interrupt",
        "Reserved Interrupt",
        "Reserved Interrupt",
        "Reserved Interrupt",
};

void add_idt_entry(uint8_t num, uint32_t offset, uint16_t segment, gate_type gate_type, uint8_t ring) {
    idt_entries[num].offset_low = offset & 0xFFFF;
    idt_entries[num].offset_high = (offset >> 16) & 0xFFFF;

    idt_entries[num].seg_select = segment;
    idt_entries[num].flags = gate_type;
    idt_entries[num].flags |= (ring << 6) & 0b01100000;
    idt_entries[num].flags |= 0b10000000;
}

void idt_install() {
    // initialize idtr
    idtr.size = (sizeof(idt_entry) * 256 - 1);
    idtr.offset = (uint32_t) &idt_entries;

    // zero our idt list so the processor is happy
    memset(&idt_entries, 0, sizeof(idt_entry) * 256);

    install_isrs();

    // call asm func to load idt
    idt_load();
}

void install_isrs() {
    for (uint8_t id = 0; id < 32; id++) {
        add_idt_entry(id, (uint32_t) isr_list[id], 0x08, INT_32, 0x00);
    }
}

void handle_fault(isr_stacktrace *r) {
    if (r->int_id < 32) {
        serial_printf(COM1, "Interrupt: %s\nHalting.\n", exception_messages[r->int_id]);
        printf("Interrupt: %s\nHalting.\n", exception_messages[r->int_id]);
        for(;;);
    }
}