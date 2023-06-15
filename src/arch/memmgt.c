//
// Created by grsan on 5/29/23.
//

#include "../include/arch/memmgt.h"
#include "../include/io/serial.h"
#include "../include/string.h"
#include "../include/math.h"

#define PAGE_SIZE 4096

mmap_page_t *pages = 0;
mmap_page_t *earliest_free_page;

uint32_t num_pages;

uint64_t usable_memory_start_address;

void* calloc(uint32_t num_blocks, uint32_t block_size) {
    uint64_t total_bytes = num_blocks * block_size;
    total_bytes += BLOCK_HEADER_SIZE;

    uint32_t required_pages = total_bytes / PAGE_SIZE;
    if (total_bytes % PAGE_SIZE > 0) required_pages++;

//    serial_printf(COM1, "Allocation of %d bytes requires %d pages\n", total_bytes, (uint64_t) required_pages);

    mmap_page_t *best_page = find_best_page(total_bytes);
    serial_printf(COM1, "best page: %d\n", (uint64_t) (best_page - pages)/ sizeof(mmap_page_t*));
    mmap_block_t *block = find_best_block(best_page, total_bytes);
    serial_printf(COM1, "best block location: 0x%16d, size: %d bytes\n", (uint64_t) block, (uint64_t) block->size);
    if (required_pages == 1) {
        block = split_block(block, total_bytes);
    } else {
        // multi-page allocation

        // find the number of remaining bytes the ending page will have, then compute the next lowest power of 2 & divide by 2
        uint32_t ending_page_new_size = (total_bytes - block->size) % 4096;
        ending_page_new_size = flp2(ending_page_new_size) >> 1;

        // update the first block on the end page to ensure it still functions properly
        mmap_page_t *ending_page = best_page + required_pages - 1;

        ending_page->first_block = (mmap_block_t *) ((uint64_t) ending_page->first_block + (4096-ending_page_new_size));
        ending_page->first_block->size = ending_page_new_size;
        ending_page->first_block->free = true;

        // update the block size so itll lead to the first block on the ending page
        block->size = ((uint32_t) ending_page->first_block) - ((uint32_t) block);

        ending_page->last_block = ending_page->first_block;

        serial_printf(COM1, "ending page firstblock addr: 0x%16d, size: %d\nending page lastblock addr: 0x%16d, size: %d\n",
                      (uint64_t) ending_page->first_block, (uint64_t) ending_page->first_block->size,
                      (uint64_t) ending_page->last_block, (uint64_t) ending_page->last_block->size);
    }
//    serial_printf(COM1, "new block location: 0x%16d, size: %d bytes\n", (uint64_t) block, (uint64_t) block->size);

    serial_printf(COM1, "zeroing 0x%16d - 0x%16d\n",
                  (uint64_t) block + BLOCK_HEADER_SIZE,
                  (uint64_t) block + BLOCK_HEADER_SIZE + ((uint64_t) total_bytes - BLOCK_HEADER_SIZE));
    memset((void*)((uint64_t) block) + BLOCK_HEADER_SIZE, 0, total_bytes - BLOCK_HEADER_SIZE);

    block->free = false;
    return (void*) block + BLOCK_HEADER_SIZE;
}

void initialize_memory_map(multiboot_memory_map_t* memory_map) {
    serial_printf(COM1, "Creating memory map...\n");
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
    uint64_t page_table_start_addr = largest_usable_entry.base_addr + PRE_MMAP_PADDING;

    serial_printf(COM1, "Adjusting start addr by 16Mib\n");
    usable_memory_start_address = page_table_start_addr + 16777216;
    uint64_t length = largest_usable_entry.length - 16777216 - PRE_MMAP_PADDING;

    num_pages = (length >> 12) - 1; // divide by 4096

    serial_printf(COM1, "Creating %d pages\n", num_pages);

    pages = (mmap_page_t*) page_table_start_addr;

    serial_printf(COM1, "Putting the page array at 0x%16d\n", pages);
    serial_printf(COM1, "sizeof page: %d, sizeof block %d\n", (uint64_t) sizeof(mmap_page_t), (uint64_t) sizeof(mmap_block_t));

    for(int i = 0; i < num_pages; i++) {
        uint32_t page_addr = (i << 12) + usable_memory_start_address; // multiply by 4096
        pages[i].length = PAGE_SIZE;
//        serial_printf(COM1, "page %d addr 0x%16d\n", (uint64_t) i, (uint64_t) page_addr);
//        serial_printf(COM1, "page %d info addr 0x%16d\n", (uint64_t) i, (uint64_t) pages + (i * sizeof(mmap_page_t)));

        pages[i].first_block = (mmap_block_t *) page_addr;
        pages[i].first_block->size = PAGE_SIZE;
        pages[i].first_block->free = true;

        pages[i].last_block = pages[i].first_block;
    }

    earliest_free_page = pages;

    serial_printf(COM1, "firstblock addr: 0x%16d, size: %d\n", (uint64_t) pages[0].first_block, (uint64_t) pages[0].first_block->size);
    serial_printf(COM1, "lastblock addr: 0x%16d, size: %d\n", (uint64_t) pages[num_pages-1].first_block,
                  (uint64_t) pages[num_pages-1].first_block->size);
}

mmap_block_t* get_next_block(mmap_block_t* block) {
    return (mmap_block_t*)((uint64_t) block + block->size);
}

mmap_block_t* split_block(mmap_block_t* block, uint32_t size) {
    if (block && size < block->size) {
        mmap_page_t *page = get_block_page(block);
        while (block->size >> 1 > size) {
            uint32_t new_size = block->size >> 1;
            block->size = new_size;

            mmap_block_t* next_block = get_next_block(block);
            next_block->size = new_size;
            next_block->free = true;

            // determine if this the first time we are splitting this block, if so set the last block ptr
            if (page->first_block == block && page->last_block == block) {
                page->last_block = next_block;
            }
        }

        if (size <= block->size) {
            return block;
        }
    }

    return NULL;
}

mmap_block_t* find_best_block(mmap_page_t *page, uint32_t size) {
    if (size > 4096)
        return page->last_block;

    mmap_block_t *block = page->first_block;
    while (!block->free || block->size < size) {
//        serial_printf(COM1, "block addr: 0x%16d, size: %d, free: %d\n", (uint64_t) block, (uint64_t) block->size, (uint64_t) block->free);
        block = get_next_block(block);
    }

    return block;
}

mmap_page_t *find_best_page(uint32_t size) {
    mmap_page_t *page = earliest_free_page;

    // state tracking vars
    uint32_t page_moved_by = 0;
    uint32_t remaining_size = size;

    for (uint32_t i = 0; i < num_pages; i++, page++) {
        if (size < PAGE_SIZE) {
            return earliest_free_page; // TODO: maybe add some way to check for free blocks of size within a page to optimize block search?
        } else {
            loop_body:
            if (!page->last_block->free) continue; // skip if the last block in this page isn't free

            // determine whether we have enough free physical space to fit this allocation
            while (remaining_size) {
                i++;
                remaining_size -= page->last_block->size;
                if ((++page)->first_block->size < ((remaining_size > 4096) ? 4096 : remaining_size)) {
                    remaining_size = size;
                    goto loop_body; // yucky goto, sorry
                }
                page_moved_by++;
                remaining_size -= ((remaining_size > 4096) ? 4096 : remaining_size);
            }

            return page - page_moved_by;
        }
    }
}

mmap_page_t *get_block_page(mmap_block_t *block) {
    // floor block addr to get page addr
    uint64_t page_addr = (uint64_t) block - ((uint64_t) block % PAGE_SIZE);

    // get the page index from its address
    uint64_t page_index = (page_addr - usable_memory_start_address) >> 12;

    return (mmap_page_t*) pages + page_index;
}

void serial_print_mmap(uint16_t serial_port, uint32_t start_page, uint32_t end_page) {
    for (uint32_t i = start_page; i < end_page; i++) {
        mmap_page_t *page = pages + i;

        serial_printf(serial_port, "Page %d: \t", (uint64_t) page);
        serial_putc(serial_port, '|');
        for (mmap_block_t *block = page->first_block; block != page->last_block; block = get_next_block(block)) {
            for (uint32_t j = 0; j < block->size / 16; j++) {
                serial_putc(serial_port, block->free ? ' ' : '*');
            }
            serial_putc(serial_port, '|');
        }
        serial_putc(serial_port, '\n');

        serial_printf(serial_port, "Addr: 0x%16d ", (uint64_t) page->first_block);
        for (mmap_block_t *block = page->first_block; block != (page + 1)->first_block; block = get_next_block(block)) {
            uint32_t size = block->size;
            uint8_t digits = 0;
            while (size != 0) {
                size /= 10;
                digits++;
            }
            serial_printf(serial_port, "%d", (uint64_t) block->size);

            for (uint32_t j = digits - 1; j < block->size / 16; j++) {
                serial_putc(serial_port, ' ');
            }
        }
        serial_printf(serial_port, "Total size %d", (uint64_t) page->length);
        serial_putc(serial_port, '\n');
    }
}