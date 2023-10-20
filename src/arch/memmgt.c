//
// Created by grsan on 5/29/23.
//

#include <arch/memmgt.h>
#include <io/serial.h>
#include <string.h>
#include <math.h>

// things seem to break with PAGE_SIZE < 4096... so keep it above that
// in bytes
#define PAGE_SIZE 4096

// font start location
extern uint8_t _binary_Tamsyn8x16b_psf_end;

mmap_page_t *pages = 0;
mmap_page_t *earliest_free_page;

uint32_t num_pages;

uint64_t usable_memory_start_address;

void* malloc(uint32_t num_bytes) {
    // TODO: new variable!
    num_bytes += BLOCK_HEADER_SIZE;

    uint32_t required_pages = num_bytes / PAGE_SIZE;
    if (num_bytes % PAGE_SIZE > 0) required_pages++;

//    serial_printf(COM1, "Allocation of %d bytes requires %d pages\n", total_bytes, (uint64_t) required_pages);

    mmap_page_t *best_page = find_best_page(num_bytes);
//    serial_printf(COM1, "best page: %d\n", (uint32_t) (best_page - pages)/ sizeof(mmap_page_t*));
    mmap_block_t *block = find_best_block(best_page, num_bytes);
//    serial_printf(COM1, "best block location: 0x%16d, size: %d bytes\n", (uint32_t) block, (uint32_t) block->size);
    if (required_pages == 1) {
        block = split_block(block, num_bytes);
    } else {
        // multi-page allocation

        for (uint32_t i = 0; i < required_pages - 2; i++) {
            (best_page + i)->flags |= ALLOCATED_OUTSIDE;
        }

        // find the number of remaining bytes the ending page will have, then compute the next lowest power of 2
        uint32_t ending_page_new_size = PAGE_SIZE - ((num_bytes - block->size) % PAGE_SIZE);
        ending_page_new_size = flp2(ending_page_new_size);

        // update the first block on the end page to ensure it still functions properly
        mmap_page_t *ending_page = best_page + required_pages - 1;

        serial_printf(COM1, "chose ending page %d\n", required_pages - 1);

        serial_printf(COM1, "ending page pre format firstblock addr: 0x%16d, size: %d\nending page lastblock addr: 0x%16d, size: %d\n",
                      (uint32_t) ending_page->first_block, (uint32_t) ending_page->first_block->size,
                      (uint32_t) ending_page->last_block, (uint32_t) ending_page->last_block->size);

        ending_page->first_block = (mmap_block_t *) ((uint32_t) ending_page->first_block + (PAGE_SIZE-ending_page_new_size));
        ending_page->first_block->size = ending_page_new_size;
        ending_page->first_block->flags |= FREE;

        // update the block size so itll lead to the first block on the ending page
        block->size = ((uint32_t) ending_page->first_block) - ((uint32_t) block);

        ending_page->last_block = ending_page->first_block;

        serial_printf(COM1, "ending page post format firstblock addr: 0x%16d, size: %d\nending page lastblock addr: 0x%16d, size: %d\n",
                      (uint32_t) ending_page->first_block, (uint32_t) ending_page->first_block->size,
                      (uint32_t) ending_page->last_block, (uint32_t) ending_page->last_block->size);
    }
//    serial_printf(COM1, "new block location: 0x%16d, size: %d bytes\n", (uint64_t) block, (uint64_t) block->size);

    block->flags &= ~FREE;
    return (void*) block + BLOCK_HEADER_SIZE;
}

void *calloc(uint32_t num_blocks, uint32_t block_size) {
    uint32_t total_bytes = num_blocks * block_size;

    void *ptr = malloc(total_bytes);
    memset(ptr, 0, total_bytes);

    return ptr;
}

void *realloc(void *ptr, uint32_t size) {
    void *ret = malloc(size);
    memcpy(ret, ptr, get_block_of_ptr((uint32_t) ptr)->size);
    free(ptr);

    return ret;
}

void free(void *ptr) {
    mmap_block_t *block = get_block_of_ptr((long) ptr);
    block->flags |= FREE;
    merge_blocks(block);
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
                      (uint32_t) entry.base_addr, (uint32_t) entry.length, (uint32_t) entry.type);

        if (entry.length > largest_usable_entry.length)
            largest_usable_entry = entry;
    }

    serial_printf(COM1, "Largest map entry: \n\tbaseaddr: 0x%16d, length: 0x%16d, type: %d\n\n",
                  (uint32_t) largest_usable_entry.base_addr, (uint32_t) largest_usable_entry.length, (uint32_t) largest_usable_entry.type);

    // we really only need 24KiB of padding, but im adding an extra 8 to play things safe
    serial_printf(COM1, "Moving page array 32KiB forward so we don't hit the stack\n");
    uint32_t page_table_start_addr = largest_usable_entry.base_addr + PRE_MMAP_PADDING;

    uint32_t font_offest = flp2((uint32_t) &_binary_Tamsyn8x16b_psf_end - 0x100000) << 1;

    // move forward to end of font object so we don't cause any issues
    serial_printf(COM1, "Moving page array forward to avoid hitting the font object (0x%16d bytes)\n", font_offest);
    page_table_start_addr += font_offest;

    serial_printf(COM1, "Adjusting start addr by 16Mib\n");
    usable_memory_start_address = page_table_start_addr + 16777216;
    uint32_t length = largest_usable_entry.length - 16777216 - PRE_MMAP_PADDING;

    num_pages = (length >> log2(PAGE_SIZE)) - 1; // divide by PAGE_SIZE

    serial_printf(COM1, "Creating %d pages\n", num_pages);

    pages = (mmap_page_t*) page_table_start_addr;

    serial_printf(COM1, "Putting the page array at 0x%16d\n", pages);
    serial_printf(COM1, "sizeof page: %d, sizeof block %d\n", (uint32_t) sizeof(mmap_page_t), (uint32_t) sizeof(mmap_block_t));

    for(uint32_t i = 0; i < num_pages; i++) {
        uint32_t page_addr = (i << log2(PAGE_SIZE)) + usable_memory_start_address; // multiply by 4096
        pages[i].first_block = (mmap_block_t *) page_addr;
        pages[i].first_block->size = PAGE_SIZE;
        pages[i].first_block->flags |= FREE;

        pages[i].last_block = pages[i].first_block;
    }

    earliest_free_page = pages;

    serial_printf(COM1, "first page firstblock addr: 0x%16d, size: %d\n", (uint32_t) pages[0].first_block,
                  (uint32_t) pages[0].first_block->size);
    serial_printf(COM1, "last page firstblock addr: 0x%16d, size: %d\n", (uint32_t) pages[num_pages-1].first_block,
                  (uint32_t) pages[num_pages-1].first_block->size);
}

mmap_block_t* get_next_block(mmap_block_t* block) {
    return (mmap_block_t*)((uint32_t) block + block->size);
}

mmap_block_t *get_prev_block(mmap_block_t *block) {
    return (mmap_block_t*)((uint32_t) block - block->size);
}

mmap_block_t* split_block(mmap_block_t* block, uint32_t size) {
    if (block && size < block->size) {
        mmap_page_t *page = get_block_page(block);
        while (block->size >> 1 > size) {
            uint32_t new_size = block->size >> 1;
            block->size = new_size;

            mmap_block_t* next_block = get_next_block(block);
            next_block->size = new_size;
            next_block->flags |= FREE | HIGH_BLOCK;

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

mmap_block_t *merge_blocks(mmap_block_t *block) {
    //TODO: add multi-page blocks here, just need to mark area as free and restore blocks at start of each page

    if (block->size > PAGE_SIZE) {
        uint32_t num_pages_to_fix = block->size / PAGE_SIZE - 1;

        // we really just need to ensure that the first block on each page is restored with accurate information
        mmap_page_t* page = get_block_page(block) + 1; // the first fully occupied page
        for (uint32_t i = 0; i < num_pages_to_fix; i++, page++) {
            page->first_block->size = PAGE_SIZE;
            page->first_block->flags |= FREE;

            page->flags &= ~ALLOCATED_OUTSIDE;
        }

        // fix ending page
        uint32_t remaining_size = PAGE_SIZE - block->size % PAGE_SIZE;

        // keep the original ending block for later, set it as a high block since that it would end up as if created naturally
        mmap_block_t *original_block = page->first_block;
        original_block->flags |= HIGH_BLOCK;

        // "new" first block
        mmap_block_t *restored_block = (mmap_block_t *) ((uint32_t) page->first_block - remaining_size);
        page->first_block = restored_block;

        // weird bitmasking masking loop to create a reasonable block structure
        uint32_t mask = PAGE_SIZE;
        bool high = false;
        while (mask > 0) {
            if (remaining_size & mask) {
                restored_block->size = remaining_size & mask;
                restored_block->flags = FREE;

                // weird mess that seems to reliably ensure the page structure is restored properly
                if (high) {
                    restored_block->flags |= HIGH_BLOCK;
                }
                high = !high;

                restored_block = get_next_block(restored_block);
            }
            mask >>= 1;
        }

        merge_blocks(original_block); // merge blocks on last page

        // update original block size
        block->size = PAGE_SIZE - ((uint32_t) block % PAGE_SIZE);
    }

    if (block->size == PAGE_SIZE) {
        return block; // no merging to be done, this block spans the entire page
    }

    mmap_block_t *merge_target;
    mmap_block_t *resulting_block;
    // different behavior based on merge direction
    if (block->flags & HIGH_BLOCK) { // merging backwards
        merge_target = (mmap_block_t*) ((uint32_t) block - block->size);
        resulting_block = merge_target;
    } else {
        merge_target = get_next_block(block);
        resulting_block = block;
    }

    // we only want to merge if this block is free AND it has no "child" blocks
    if (merge_target->flags & FREE && merge_target->size == block->size) {
        resulting_block->size = block->size << 1; // fix size
        return merge_blocks(resulting_block); // merge upwards as far as we can go
    }

    return block;
}

mmap_block_t* find_best_block(mmap_page_t *page, uint32_t size) {
    if (size > PAGE_SIZE)
        return page->last_block;

    mmap_block_t *block = page->first_block;
    while (!(block->flags & FREE) || block->size < size) {
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
        if (size < PAGE_SIZE) break;

        loop_body:
        // skip if the last block in this page isn't free or if it's already allocated to an outside pointer
        if (!(page->last_block->flags & FREE) || page->flags & ALLOCATED_OUTSIDE) continue;

        // determine whether we have enough free physical space to fit this allocation
        while (remaining_size) {
            i++;
            remaining_size -= page->last_block->size;
            if ((++page)->first_block->size < ((remaining_size > PAGE_SIZE) ? PAGE_SIZE : remaining_size)) {
                remaining_size = size;
                goto loop_body; // yucky goto, sorry
            }
            page_moved_by++;
            remaining_size -= ((remaining_size > PAGE_SIZE) ? PAGE_SIZE : remaining_size);
        }

        return page - page_moved_by;
    }

    return earliest_free_page;
}

mmap_page_t *get_block_page(mmap_block_t *block) {
    // floor block addr to get page addr
    uint32_t page_addr = (uint32_t) block - ((uint32_t) block % PAGE_SIZE);

    // get the page index from its address
    uint32_t page_index = (page_addr - usable_memory_start_address) >> log2(PAGE_SIZE);

    return (mmap_page_t*) pages + page_index;
}

mmap_block_t *get_block_of_ptr(uint32_t addr) {
    return (mmap_block_t*) (addr - BLOCK_HEADER_SIZE);
}