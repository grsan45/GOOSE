//
// Created by grsan on 5/29/23.
//

#include "../include/arch/memmgt.h"
#include "../include/io/serial.h"
#include "../include/string.h"

#define PAGE_SIZE 4096

mmap_page_t *pages = 0;
mmap_page_t earliest_free_page;

void* calloc(uint32_t num_blocks, uint32_t block_size) {
    uint64_t total_bytes = num_blocks * block_size;
    uint32_t required_pages = total_bytes / PAGE_SIZE;
    if (total_bytes % PAGE_SIZE > 0) required_pages++;

//    serial_printf(COM1, "Allocation of %d bytes requires %d pages\n", total_bytes, (uint64_t) required_pages);

    uint32_t required_block_header_size = required_pages * BLOCK_HEADER_SIZE; // for allocations that require multiple pages
    mmap_block_t* block = find_best_block(total_bytes + required_block_header_size);
    serial_printf(COM1, "best block location: 0x%16d, size: %d bytes\n", (uint64_t) block, (uint64_t) block->size);
    block = split_block(block, total_bytes + required_block_header_size);
    serial_printf(COM1, "new block location: 0x%16d, size: %d bytes\n", (uint64_t) block, (uint64_t) block->size);

    memset(block + BLOCK_HEADER_SIZE, 0, total_bytes);

    block->free = false;
    return (void*) block + BLOCK_HEADER_SIZE;
}

void initialize_memory_map(multiboot_memory_map_t* memory_map) {
    uint32_t num_sectors = (memory_map->size-16)/memory_map->entry_size;

    // find the largest usable entry, maybe take advantage of others once paging is implemented?
    multiboot_memory_map_entry_t largest_usable_entry = {.length = 0};
    for (uint32_t i = 0; i < num_sectors; i++) {
        multiboot_memory_map_entry_t entry = memory_map->entries[i];
        if (entry.type != 1) continue; // cant use anything else

        serial_printf(COM1, "Found map entry: \n\tbaseaddr: 0x%16d, length: 0x%16d, type: %d\n\n",
                      entry.base_addr, entry.length, (uint64_t) entry.type);

        if (entry.length > largest_usable_entry.length)
            largest_usable_entry = entry;
    }

    serial_printf(COM1, "Largest map entry: \n\tbaseaddr: 0x%16d, length: 0x%16d, type: %d\n\n",
                  largest_usable_entry.base_addr, largest_usable_entry.length, largest_usable_entry.type);

    // we really only need 24KiB of padding, but im adding an extra 8 to play things safe
    serial_printf(COM1, "Moving page array 32KiB forward so we don't hit the stack\n");
    uint64_t page_table_start_addr = largest_usable_entry.base_addr + 32768;

    serial_printf(COM1, "Adjusting start addr by 16Mib\n");
    uint64_t start_addr = page_table_start_addr + 16777216;
    uint64_t length = largest_usable_entry.length - 16777216 - 32768;

    uint32_t num_pages = (length >> 12) - 1; // divide by 4096

    serial_printf(COM1, "Creating %d pages\n", num_pages);

    pages = (mmap_page_t*) page_table_start_addr;

    serial_printf(COM1, "Putting the page array at 0x%16d\n", pages);
    serial_printf(COM1, "sizeof page: %d, sizeof block %d\n", (uint64_t) sizeof(mmap_page_t), (uint64_t) sizeof(mmap_block_t));

    for(int i = 0; i < num_pages; i++) {
        uint32_t page_addr = (i << 12) + start_addr; // multiply by 4096
        pages[i].length = PAGE_SIZE;
        pages[i].free = true;
//        serial_printf(COM1, "page %d addr 0x%16d\n", (uint64_t) i, (uint64_t) page_addr);
//        serial_printf(COM1, "page %d info addr 0x%16d\n", (uint64_t) i, (uint64_t) pages + (i * sizeof(mmap_page_t)));

        pages[i].first_block = (mmap_block_t *) page_addr;
        pages[i].first_block->size = PAGE_SIZE;
        pages[i].first_block->free = true;
    }

    earliest_free_page = pages[0];

    serial_printf(COM1, "firstblock addr: 0x%16d, size: %d\n", (uint64_t) pages[0].first_block, (uint64_t) pages[0].first_block->size);
    serial_printf(COM1, "lastblock addr: 0x%16d, size: %d\n", (uint64_t) pages[num_pages-1].first_block,
                  (uint64_t) pages[num_pages-1].first_block->size);

    return;
}

mmap_block_t* get_next_block(mmap_block_t* block) {
    return (mmap_block_t*)((uint64_t) block + block->size);
}

mmap_block_t* split_block(mmap_block_t* block, uint32_t size) {
    if (block && size < block->size) {
//        serial_printf(COM1, "splitting block at 0x%16d\n", (uint64_t) block);
        while (block->size >> 1 > size) {
            uint32_t new_size = block->size >> 1;
//            serial_printf(COM1, "test %d, %d, %d\n", (uint64_t) size, (uint64_t) block->size, (uint64_t) new_size);
            block->size = new_size;
            mmap_block_t* next_block = get_next_block(block);
            next_block->size = new_size;
            next_block->free = true;
        }

        if (size <= block->size) {
            return block;
        }
    }

    return NULL;
}

mmap_block_t* find_best_block(uint32_t size) {
    mmap_block_t *block = earliest_free_page.first_block;
    while (!block->free || block->size < size) {
        block = get_next_block(block);
    }

    return block;
}