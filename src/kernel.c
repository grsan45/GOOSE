#include "./include/arch/gdt.h"
#include "./include/arch/idt.h"

#include "./include/boot/multiboot2.h"
#include "./include/display/framebuffer.h"
#include "./include/display/psf_font.h"
#include "./include/arch/memmgt.h"

#include "./include/io/timer.h"
#include "./include/io/keyboard.h"

#include "./include/io/serial.h"

void kmain(uint32_t magic, uint32_t multiboot_addr) {
    // init serial for debugging COM1, if it fails then panic... or smth
    if (init_serial(COM1)) {
        for(;;);
    }

    serial_printf(COM1, "Starting output to COM1\n");

    // initialize gdt & idt
    gdt_install();
    idt_install();

    setup_timer();

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
            info.addr = (void *) (long) framebuffer_tag->common.framebuffer_addr;
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
            serial_printf(COM1, "framebuffer location: 0x%16d\n",(uint32_t)info.addr);
        }

        tag = (multiboot_tag_t*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    // prepare framebuffer for use
    init_psf();
    clear();

    printf("Hello world!!\n");

    // test calloc
    char* ptr = calloc(119, sizeof(char));
    char* ptr2 = calloc(4096, sizeof(char));
    char* ptr3 = calloc(512, sizeof(char));
    char* ptr4 = calloc(424, sizeof(char));
    char* ptr5 = calloc(24576, sizeof(char));

    ptr3 = realloc(ptr3, 1024);

    printf("Test pointer 1 located at 0x%16d, block size: %d\n", (uint32_t) ptr,
                  (uint32_t) get_block_of_ptr((uint32_t) ptr)->size);
    printf("Test pointer 2 located at 0x%16d, block size: %d\n", (uint32_t) ptr2,
                  (uint32_t) get_block_of_ptr((uint32_t) ptr2)->size);
    printf("Test pointer 3 located at 0x%16d, block size: %d\n", (uint32_t) ptr3,
                  (uint32_t) get_block_of_ptr((uint32_t) ptr3)->size);
    printf("Test pointer 4 located at 0x%16d, block size: %d\n", (uint32_t) ptr4,
                  (uint32_t) get_block_of_ptr((uint32_t) ptr4)->size);
    printf("Test pointer 5 located at 0x%16d, block size: %d\n", (uint32_t) ptr5,
                  (uint32_t) get_block_of_ptr((uint32_t) ptr5)->size);

    mmap_block_t *ptr1_block = (mmap_block_t *) ((uint32_t) ptr - BLOCK_HEADER_SIZE);
    printf("PRE FREE PTR 1 block properties -- free: %d, block size: %d\n", ptr1_block->flags & FREE, ptr1_block->size);
    free(ptr);
    printf("POST FREE PTR 1 block properties -- free: %d, block size: %d\n", ptr1_block->flags & FREE, ptr1_block->size);
    free(ptr4);

    mmap_block_t *ptr5_block = (mmap_block_t *) ((uint32_t) ptr5 - BLOCK_HEADER_SIZE);
    printf("PRE FREE PTR 5 block properties -- free: %d, block size: %d\n", ptr5_block->flags & FREE, ptr5_block->size);
    free(ptr5);
    printf("POST FREE PTR 5 block properties -- free: %d, block size: %d\n", ptr5_block->flags & FREE, ptr5_block->size);

    free(ptr3);
    free(ptr2);

    printf("All pointers freed\n");

    // allow user input now
    setup_keyboard();

    for(;;);
}