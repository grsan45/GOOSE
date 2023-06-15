//
// Created by grsan on 5/29/23.
//

#ifndef GOOSE_MEMMGT_H
#define GOOSE_MEMMGT_H

#include <stdint.h>
#include <stdbool.h>

#include "../boot/multiboot2.h"

#define BLOCK_HEADER_SIZE sizeof(mmap_block_t)
#define PRE_MMAP_PADDING (16384 + 8192 + 8192) // 16KiB for the stack + 8KiB stack offset + 8KiB jic

typedef struct s_mmap_block_t {
    uint32_t size;
    bool free; // TODO: replace this with owner proc when implemented
} mmap_block_t;

typedef struct {
    mmap_block_t *first_block;
    mmap_block_t *last_block; // used for allocations spanning multiple pages,
                              // if this is free then this page is eligible for one of those allocations
    uint64_t length; // should be a power of 2
} mmap_page_t;

void initialize_memory_map(multiboot_memory_map_t* memory_map);
void* calloc(uint32_t num_blocks, uint32_t block_size);

mmap_block_t *split_block(mmap_block_t* block, uint32_t size);

mmap_block_t *get_next_block(mmap_block_t *block);
mmap_block_t *find_best_block(mmap_page_t *page, uint32_t size);

mmap_page_t *find_best_page(uint32_t size);

mmap_page_t *get_block_page(mmap_block_t *block);

void serial_print_mmap(uint16_t serial_port, uint32_t start_page, uint32_t end_page);

#endif //GOOSE_MEMMGT_H
