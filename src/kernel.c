#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "include/vga.h"
#include "include/gdt.h"

void kmain() {
    gdt_install();

    init_vga_terminal();

    vga_print("Hello world!\n");
}