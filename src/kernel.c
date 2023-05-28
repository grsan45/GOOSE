#include "src/include/arch/gdt.h"
#include "src/include/arch/idt.h"

#include "src/include/boot/multiboot2.h"
#include "src/include/display/framebuffer.h"

void kmain(uint32_t magic, uint32_t multiboot_addr) {
    gdt_install();
    idt_install();

    multiboot_tag *tag = (multiboot_tag*) multiboot_addr+8;

    if (magic != 0x36d76289) {
        return;
    }

    if (multiboot_addr & 7) {
        return;
    }

    while (tag->type != 0) {
        if (tag->type == 8) {
            multiboot_framebuffer_tag *framebuffer_tag = (multiboot_framebuffer_tag*) tag;
            framebuffer_info info;
            info.addr = framebuffer_tag->common.framebuffer_addr;
            info.width = framebuffer_tag->common.framebuffer_width;
            info.height = framebuffer_tag->common.framebuffer_height;
            info.pitch = framebuffer_tag->common.framebuffer_pitch;
            info.depth = framebuffer_tag->common.framebuffer_bpp;

            if (framebuffer_tag->common.type == 1) {
                info.red_location = framebuffer_tag->red_location;
                info.red_bit_depth = framebuffer_tag->red_bit_depth;
                info.green_location = framebuffer_tag->green_location;
                info.green_bit_depth = framebuffer_tag->green_bit_depth;
                info.blue_location = framebuffer_tag->blue_location;
                info.blue_bit_depth = framebuffer_tag->blue_bit_depth;
            }

            *(uint8_t*)(info.addr) = 0xFF;
            init_framebuffer(info);
        }

        tag = (multiboot_tag*)((uint8_t*)tag+((tag->size + 7) & ~7));
    }
}