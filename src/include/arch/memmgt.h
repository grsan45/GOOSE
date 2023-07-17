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

#define FREE 0b00000001
#define HIGH_BLOCK 0b00000010

#define ALLOCATED_OUTSIDE 0b00000001

typedef struct s_mmap_block_t {
    uint32_t size;
    uint8_t flags; // | 1-6 unused | 7 high block | 8 free |
} mmap_block_t;

typedef struct {
    mmap_block_t *first_block;
    mmap_block_t *last_block; // used for allocations spanning multiple pages,
                              // if this is free then this page is eligible for one of those allocations
    uint8_t flags; // 1-7 unused | 8 allocated to pointer outside this page
} mmap_page_t;

void initialize_memory_map(multiboot_memory_map_t* memory_map);
void* malloc(uint32_t num_bytes);
void* calloc(uint32_t num_blocks, uint32_t block_size);
void free(void* ptr);

mmap_block_t *split_block(mmap_block_t* block, uint32_t size);
mmap_block_t *merge_blocks(mmap_block_t*);

mmap_block_t *get_next_block(mmap_block_t *block);
mmap_block_t *find_best_block(mmap_page_t *page, uint32_t size);

mmap_page_t *find_best_page(uint32_t size);

mmap_page_t *get_block_page(mmap_block_t *block);
mmap_block_t *get_block_of_ptr(uint32_t addr);

#endif //GOOSE_MEMMGT_H
