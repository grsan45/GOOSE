#include "./include/arch/gdt.h"
#include "./include/arch/idt.h"

#include "./include/boot/multiboot2.h"
#include "./include/display/framebuffer.h"
#include "./include/arch/memmgt.h"

#include "./include/io/serial.h"

void kmain(uint32_t magic, uint32_t multiboot_addr) {
    // init serial for debugging COM1, if it fails then panic... or smth
    if (init_serial(COM1)) {
        for(;;);
    }

    serial_printf(COM1, "Hello serial world!\n");

    // initialize gdt & idt
    gdt_install();
    idt_install();

    serial_printf(COM1, "Multiboot addr: 0x%16d\n", multiboot_addr);

    // memory info tag
    multiboot_memory_map_t *memory_map_tag;

    // framebuffer tag
    multiboot_framebuffer_tag_t *framebuffer_tag;

    // validate multiboot header
    if (magic != 0x36d76289) {
        return;
    }

    if (multiboot_addr & 7) {
        return;
    }

    // move multiboot header to somewhere it can be undisturbed while the memory map is created
    multiboot_tag_t *tag = (multiboot_tag_t*) memcpy(((void*)0x1000), (void*)multiboot_addr, *((uint32_t*)multiboot_addr)) + 8;

    // get multiboot tags, or at least the ones I care about
    while (tag->type != 0) {
        if (tag->type == 6) {
            memory_map_tag = (multiboot_memory_map_t *) tag;
            initialize_memory_map(memory_map_tag);
        }
        if (tag->type == 8) {
            framebuffer_tag = (multiboot_framebuffer_tag_t *) tag;
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

            *(uint8_t *) (info.addr) = 0xFF;
            init_framebuffer(info);

            // TODO: mark the vram as used in the memory map
            serial_printf(COM1, "framebuffer location: 0x%16d\n",(uint64_t)info.addr);
        }

        tag = (multiboot_tag_t*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    serial_printf(COM1, "Multiboot addr: 0x%16d\n", (uint64_t) tag);

    // test framebuffer
    put_pixel(0, 0, 0xffa600);

    // test calloc
    char* ptr = calloc(128, sizeof(char));
    serial_printf(COM1, "Test pointer located at 0x%16d\n", ptr);

    char* ptr2 = calloc(512, sizeof(char));
    serial_printf(COM1, "Test pointer 2 located at 0x%16d\n", ptr2);
}