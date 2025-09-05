#include <stdlib.h>

#include "../include/filetable.h"

int32_t init_ft(file_table *table) {
    table->entries = malloc(sizeof(ft_entry) * MAX_FILES); // todo: realloc if we run out of files
    if (table->entries <= 0)
        return -1;

    table->count = 0;

    return 0;
}

int32_t create_entry(file_table *table, pid_t proc, uint32_t fd, uint8_t access_flags, uint8_t type_flags) {
    uint32_t index = FD_HASH(proc, fd) % MAX_FILES;
    table->entries[index].access_flags = access_flags;
    table->entries[index].type_flags = access_flags;

    return 0;
}

ft_entry find_entry(file_table *table, pid_t proc, uint32_t fd) {
    uint32_t index = FD_HASH(proc, fd) % MAX_FILES;
    ft_entry entry = table->entries[index];

    if (entry.access_flags == 0 || entry.type_flags == 0)
        return INVALID_ENTRY;

    return entry;
}