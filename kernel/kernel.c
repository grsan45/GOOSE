#include <arch/gdt.h>
#include <arch/idt.h>
#include <arch/syscalls.h>

#include <boot/multiboot2.h>
#include <display/framebuffer.h>
#include <display/psf_font.h>
#include <arch/memmgt.h>

#include "include/fs/filetable.h"
#include "include/driver/klog_driver.h"

#include <io/timer.h>
#include <io/keyboard.h>

#include <io/serial.h>

#include "./include/mod/kmod.h"

#include <cpuid.h>
#include <arch/cpu.h>

#include <sys/syscall.h>
#include <stdio.h>

// configuration

#define MAX_MODULES 16

// font start location
extern uint8_t _binary_Tamsyn8x16b_psf_end;

file_table main_ft = {};

void kmain(uint32_t magic, uint32_t multiboot_addr) {
    // init serial for debugging COM1, if it fails then panic... or smth
    if (init_serial(COM1)) {
        for(;;);
    }

    // check apic availability
    uint32_t eax, edx, unused;
    __get_cpuid(1, &eax, &unused, &unused, &edx);
    if (!(edx | CPUID_FEAT_EDX_APIC)) {
        serial_printf(COM1, "APIC Not supported, refusing to continue.");
        for(;;);
    }

    serial_printf(COM1, "Starting output to COM1\n");

    // initialize gdt & idt
    gdt_install();
    idt_install();

    // initialize page structures
    serial_printf(COM1, "Initializing new page directory\n");

    setup_timer();
    setup_syscall_handler();

    // memory info tag
    multiboot_memory_map_t *memory_map_tag;

    // framebuffer tag
    multiboot_framebuffer_tag_t *framebuffer_tag;

    // Module vars. Not a constant value.
    multiboot_module_tag_t *mod_tag;

    uint32_t mod_count = 0;
    kmod_t mods[MAX_MODULES] = {0};

    // validate multiboot header
    if (magic != 0x36d76289 || multiboot_addr & 7) {
        return;
    }

    // move multiboot header to somewhere it can be undisturbed while the memory map is created
    multiboot_tag_t *tag = (multiboot_tag_t*) memcpy((void*)0x1000, (void*)multiboot_addr, *(uint32_t*)multiboot_addr) + 8;

    // get multiboot tags, or at least the ones I care about
    while (tag->type != 0) {
        switch (tag->type)
        {
        case MULTIBOOT_TAG_MODULES:
            mod_tag = (multiboot_module_tag_t *) tag;

            mods[mod_count++] = (kmod_t){
                .start = mod_tag->mod_start,
                .end = mod_tag->mod_end,
                .name = &mod_tag->string
            };

            break;
        case MULTIBOOT_TAG_MEM_MAP:
            memory_map_tag = (multiboot_memory_map_t *) tag;
            initialize_memory_map(memory_map_tag, _binary_Tamsyn8x16b_psf_end);

            break;
        case MULTIBOOT_TAG_FRAMEBUFFER:
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

            break;
        default:
            break;
        }

        tag = (multiboot_tag_t*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    // initialize file table
    init_ft(&main_ft);

    // set up klog file driver
    file_driver klog_file_driver = {.write=klog_write,.read=klog_read};

    // set up klog node
    fs_node *log_node = malloc(sizeof(fs_node));
    log_node->name="klog";
    log_node->type_flags = TYPE_DEVICE;
    log_node->driver = &klog_file_driver;

    // Create kernel stdout. Will print to framebuffer first, serial later, then kernel log file.
    create_entry(&main_ft, 0, 1, log_node, FREAD | FWRITE, TYPE_DEVICE);

    // prepare framebuffer for use
    init_psf();
    clear();

    printf("Starting GOOSE\n");

    for (uint32_t i = 0; i < mod_count; i++)
    {
        printf("Found module %s at %x\n", mods[i].name, (uint32_t)mods[i].start);
    }

    // allow user input now
    setup_keyboard();

    printf("%s", "End of kmain. Halting.\n");

    for(;;);
}