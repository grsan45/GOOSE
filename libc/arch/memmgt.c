//
// Created by grsan on 5/29/23.
//

#include <arch/memmgt.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// things seem to break with PAGE_SIZE < 4096... so keep it above that
// in bytes
#define PAGE_SIZE 4096


mmap_page_t *pages = 0;
mmap_page_t *start_page;

uint32_t num_pages;

uint64_t usable_memory_start_address;

void* malloc(uint32_t num_bytes) {
    num_bytes += BLOCK_HEADER_SIZE;

    uint32_t required_pages = num_bytes / PAGE_SIZE;
    if (num_bytes % PAGE_SIZE > 0) required_pages++;


    mmap_page_t *best_page = find_best_page(num_bytes);
    mmap_block_t *block = find_best_block(best_page, num_bytes);
    if (required_pages == 1) {
        block = split_block(block, num_bytes);
        best_page->flags |= OCCUPIED;
    } else {
        // multi-page allocation

        // set appropriate flags on all used pages
        for (uint32_t i = 0; i < required_pages - 2; i++) {
            (best_page + i)->flags |= ALLOCATED_OUTSIDE | OCCUPIED;
        }

        // find the number of remaining bytes the ending page will have, then compute the next lowest power of 2
        uint32_t ending_page_new_size = PAGE_SIZE - ((num_bytes - block->size) % PAGE_SIZE);
        ending_page_new_size = flp2(ending_page_new_size);

        // update the first block on the end page to ensure it still functions properly
        mmap_page_t *ending_page = best_page + required_pages - 1;

        ending_page->first_block = (mmap_block_t *) ((uint32_t) ending_page->first_block + (PAGE_SIZE-ending_page_new_size));
        ending_page->first_block->size = ending_page_new_size;
        ending_page->first_block->flags |= FREE;

        // update the block size so itll lead to the first block on the ending page
        block->size = ((uint32_t) ending_page->first_block) - ((uint32_t) block);

        ending_page->last_block = ending_page->first_block;
    }

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

void initialize_memory_map(multiboot_memory_map_t* memory_map, uint8_t font_start_location) {
    uint32_t num_sectors = (memory_map->size-16)/memory_map->entry_size;

    // find the largest usable entry, maybe take advantage of others once paging is implemented?
    multiboot_memory_map_entry_t largest_usable_entry = {.length = 0};
    for (uint32_t i = 0; i < num_sectors; i++) {
        multiboot_memory_map_entry_t entry = memory_map->entries[i];
        if (entry.type != 1) continue; // cant use anything else

        if (entry.length > largest_usable_entry.length)
            largest_usable_entry = entry;
    }


    // we really only need 24KiB of padding, but im adding an extra 8 to play things safe
    uint32_t page_table_start_addr = largest_usable_entry.base_addr + PRE_MMAP_PADDING;

    uint32_t font_offest = flp2((uint32_t) &font_start_location - 0x100000) << 1;

    // move forward to end of font object so we don't cause any issues
    page_table_start_addr += font_offest;

    usable_memory_start_address = page_table_start_addr + 16777216;
    uint32_t length = largest_usable_entry.length - 16777216 - PRE_MMAP_PADDING;

    num_pages = (length >> log2(PAGE_SIZE)) - 1; // divide by PAGE_SIZE

    pages = (mmap_page_t*) page_table_start_addr;


    for(uint32_t i = 0; i < num_pages; i++) {
        uint32_t page_addr = (i << log2(PAGE_SIZE)) + usable_memory_start_address; // multiply by PAGE_SIZE
        pages[i].flags = 0;
        pages[i].first_block = (mmap_block_t *) page_addr;
        pages[i].first_block->size = PAGE_SIZE;
        pages[i].first_block->flags |= FREE;

        pages[i].last_block = pages[i].first_block;
    }

    start_page = pages;
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
    if (block->size > PAGE_SIZE) {
        uint32_t num_pages_to_fix = block->size / PAGE_SIZE - 1;

        // we really just need to ensure that the first block on each page is restored with accurate information
        mmap_page_t* page = get_block_page(block) + 1; // the first fully occupied page
        for (uint32_t i = 0; i < num_pages_to_fix; i++, page++) {
            page->first_block->size = PAGE_SIZE;
            page->first_block->flags |= FREE;

            page->flags ^= ALLOCATED_OUTSIDE;
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

    // clear the occupied flag if this block is restored to a full page length
    if (block->size == PAGE_SIZE)
        get_block_page(block)->flags ^= OCCUPIED;

    return block;
}

mmap_block_t* find_best_block(mmap_page_t *page, uint32_t size) {
    if (size > PAGE_SIZE)
        return page->last_block;

    mmap_block_t *block = page->first_block;
    while (!(block->flags & FREE) || block->size < size) {
        block = get_next_block(block);
    }

    return block;
}

mmap_page_t *find_best_page(uint32_t size)
{
    // note: this function does necessarily not find the perfect page for any given size,
    // it just finds one that is *good enough* in a relatively shorter timeframe.

    mmap_page_t *selected_page = start_page;

    // loop through all pages. Will return when an appropriate page is found
    // todo: be more efficient about this.
    for (; selected_page < &pages[num_pages-1]; selected_page++)
    {
        if (selected_page->flags & ALLOCATED_OUTSIDE || !(selected_page->last_block->flags & FREE)) continue;

        // check for contiguous empty pages in case we are doing a multi-page allocation
        if (size > PAGE_SIZE)
        {
            uint32_t required_extra_page_count = (size - selected_page->last_block->size) / PAGE_SIZE + 1;
            bool all_free = true;

            for (uint32_t i = 1; i < required_extra_page_count; i++)
            {
                // if any of the required blank pages have any flags set, then our selected page isn't usable
                if ((selected_page + i)->flags > 0) { all_free = false; break; }
            }

            if (!all_free) continue;
        } else
        {
            // for single-page allocations, just check that the last block has appropriate space.
            // The allocator function will be more efficient if possible
            if (selected_page->last_block->size < size) continue;
        }

        return selected_page;
    }

    return NULL;
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