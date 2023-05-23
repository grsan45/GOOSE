#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "include/vga.h"

void kmain() {
    init_vga_terminal();

    vga_print("Hello world!\n");
}