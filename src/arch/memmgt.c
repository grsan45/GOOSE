//
// Created by grsan on 5/29/23.
//

#include "../include/arch/memmgt.h"
#include "../include/io/serial.h"
#include "../include/string.h"

#define MMAP_TAG_METADATA_SIZE 16

mmap_page_t *pages = 0;

void calloc(uint32_t num_blocks, uint32_t block_size) {

}

void initialize_memory_map(multiboot_memory_map_t* memory_map) {
    uint32_t num_sectors = (memory_map->size-16)/memory_map->entry_size;
    char digitbuffer[64] = {0};

    // find the largest usable entry, maybe take advantage of others once paging is implemented?
    multiboot_memory_map_entry_t largest_usable_entry = {.length = 0};
    for (uint32_t i = 0; i < num_sectors; i++) {
        multiboot_memory_map_entry_t entry = memory_map->entries[i];
        if (entry.type != 1) continue; // cant use anything else

        serial_printf(COM1, "Found map entry: \n\tbaseaddr: 0x");
        serial_printf(COM1, itoa(entry.base_addr, digitbuffer, 16));
        serial_printf(COM1, ", length: ");
        serial_printf(COM1, itoa(entry.length, digitbuffer, 16));
        serial_printf(COM1, ", type: ");
        serial_printf(COM1, itoa(entry.type, digitbuffer, 10));
        serial_printf(COM1, "\n\n");

        if (entry.length > largest_usable_entry.length)
            largest_usable_entry = entry;
    }

    serial_printf(COM1, "Largest map entry: \n\tbaseaddr: 0x");
    serial_printf(COM1, itoa(largest_usable_entry.base_addr, digitbuffer, 16));
    serial_printf(COM1, ", length: ");
    serial_printf(COM1, itoa(largest_usable_entry.length, digitbuffer, 10));
    serial_printf(COM1, ", type: ");
    serial_printf(COM1, itoa(largest_usable_entry.type, digitbuffer, 10));
    serial_printf(COM1, "\n");

    serial_printf(COM1, "Adjusting start addr by 16Mib");
    uint64_t start_addr = largest_usable_entry.base_addr + 16777216;
    uint64_t length = largest_usable_entry.length - 16777216;

    uint32_t num_pages = length >> 12; // divide by 4096

    serial_printf(COM1, "Creating ");
    serial_printf(COM1, itoa(num_pages, digitbuffer, 10));
    serial_printf(COM1, " pages\n");

    pages = (mmap_page_t*) start_addr;

    serial_printf(COM1, "Putting the page array at 0x");
    serial_printf(COM1, itoa((uint64_t)pages, digitbuffer, 16));
    serial_printf(COM1, "\n");

    for(int i = 0; i < num_pages; i++) {
        pages[i].start_addr = i << 12; // multiply by 4096
        pages[i].length = 4096;
        pages[i].root_block.location = i << 12;
        pages[i].root_block.free = true;
        pages[i].root_block.size = 4096;
        pages[i].root_block.buddy = 0;
        pages[i].root_block.left = 0;
        pages[i].root_block.right = 0;
    }

    serial_printf(COM1, "MMap location: 0x");
    serial_printf(COM1, itoa((uint64_t)pages, digitbuffer, 16));
    serial_printf(COM1, "\n");

    return;
}