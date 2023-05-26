#include "include/vga.h"
#include "include/gdt.h"
#include "include/idt.h"

void kmain() {
    init_vga_terminal();

    gdt_install();
    idt_install();

    vga_print("Hello kernel world!\n");
//    vga_print("Testing DBZ by calling __asm__(\"div %ah\");\n");
//    __asm__ ("div %ah");
//    vga_print("Shouldn't get here.\n");
//    vga_putc(100/0);
}

void asm_debug() {
    vga_print("debug\n");
}