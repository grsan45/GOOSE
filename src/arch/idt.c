//
// Created by grsan on 5/25/23.
//

#include "../include/arch/idt.h"
#include "../include/arch/isrs.h"
#include "../include/string.h"
#include "../include/io/serial.h"
#include "../include/display/framebuffer.h"
#include "../include/io/ioutils.h"

#define PIC1_CMD 0x20
#define PIC1_DATA 0x21
#define PIC2_CMD 0xA0
#define PIC2_DATA 0xA1

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

void set_irq_handler(uint8_t irq, void (*handler)(isr_stacktrace *r)) {
    irq_handlers[irq] = handler;
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

    // setup pics and fix vector map
    outb(PIC1_CMD, 0x11); // initialize command (0x10) | 4 initialization words (0x01)
    outb(PIC2_CMD, 0x11); // initialize command (0x10) | 4 initialization words (0x01)
    outb(PIC1_DATA, 0x20); // master pic vector offset
    outb(PIC2_DATA, 0x28); // slave pic vector offset
    outb(PIC1_DATA, 0x04); // tell master pic that slave pic is at irq2 (0x04)
    outb(PIC2_DATA, 0x02); // tell slave pic we're using single cascade mode
    outb(PIC1_DATA, 0x01); // use 8086 mode
    outb(PIC2_DATA, 0x01); // use 8086 mode
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);

    for (uint8_t id = 0; id < 32; id++) {
        add_idt_entry(id, (uint32_t) isr_list[id], 0x08, INT_32, 0x00);
    }

    for (uint8_t id = 0; id < 16; id++) {
        add_idt_entry(id + 32, (uint32_t) irq_list[id], 0x08, INT_32, 0x00);
    }
}

void handle_fault(isr_stacktrace *r) {
    if (r->int_id < 32) {
        serial_printf(COM1, "Interrupt: %s\nHalting.\n", exception_messages[r->int_id]);
        printf("Interrupt: %s\nHalting.\n", exception_messages[r->int_id]);
        for(;;);
    }
}

void handle_irq(isr_stacktrace *r) {
    void (*handler)(isr_stacktrace *r);

    // if we have a handler for this irq
    handler = irq_handlers[r->int_id - 32];
    if (handler) handler(r);

    // send end of interrupt instruction to slave PIC if irq8-15
    if (r->int_id >= 40) {
        outb(PIC2_CMD, 0x20);
    }

    // send end of interrupt to master pic
    outb(PIC1_CMD, 0x20);
}