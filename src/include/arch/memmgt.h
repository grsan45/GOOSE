//
// Created by grsan on 5/29/23.
//

#ifndef GOOSE_MEMMGT_H
#define GOOSE_MEMMGT_H

#include <stdint.h>
#include <stdbool.h>

#include "../boot/multiboot2.h"

typedef struct s_mmap_block_t{
    struct s_mmap_block_t* buddy;
    struct s_mmap_block_t* left;
    struct s_mmap_block_t* right;

    uint64_t location;
    uint64_t size;
    bool free; // TODO: replace this with owner proc when implemented
} mmap_block_t;

typedef struct {
    uint64_t start_addr;
    uint64_t length; // should be a power of 2
    mmap_block_t root_block;
} mmap_page_t;

void initialize_memory_map(multiboot_memory_map_t* memory_map);
void calloc(uint32_t num_blocks, uint32_t block_size);

#endif //GOOSE_MEMMGT_H
