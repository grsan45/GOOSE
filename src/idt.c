//
// Created by grsan on 5/25/23.
//

#include "include/idt.h"
#include "include/string.h"

idt_entry idt_entries[256];
idt_ptr idtr;

static const char* exception_messages[32] = {
        "Division By Zero.",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "Double fault.",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
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

#include "include/vga.h"

void install_isrs() {
    add_idt_entry(0, (unsigned)isr0, 0x08, INT_32, 0x00);
    add_idt_entry(8, (unsigned)isr8, 0x08, INT_32, 0x00);
}

void handle_fault(isr_stacktrace *r) {
    if (r->int_id < 32) {
        vga_print("Interrupt: ");
        vga_print(exception_messages[r->int_id]);
        vga_print("\n");
        vga_print("Halting.");
        while(1);
    }
}