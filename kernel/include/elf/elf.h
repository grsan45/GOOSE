#ifndef GOOSE_ELF_H
#define GOOSE_ELF_H

#include <stdlib.h>

#define BIT_32          1
#define BIT_64          2

#define LITTLE_ENDIAN   1
#define BIG_ENDIAN      2

#define VERSION         1

#define SUPPORTED_ISA   3

typedef struct __attribute__((__packed__))
{
    uint32_t    magic;
    uint8_t     class;
    uint8_t     endian;
    uint8_t     version;
    uint8_t     abi;
    uint8_t     abi_version;
    uint8_t     __padding[7];
    uint16_t    file_type;
    uint16_t    isa;
    uint32_t    entry_point; // todo: 64-bit support
    uint32_t    ph_location;
    uint32_t    sh_table_location;
    uint32_t    flags;
    uint16_t    size;
    uint16_t    ph_entry_size;
    uint16_t    ph_entry_count;
    uint16_t    sh_entry_size;
    uint16_t    sh_entry_count;
    uint16_t    sh_str_index;
} elf_header_t;

#endif //GOOSE_ELF_H