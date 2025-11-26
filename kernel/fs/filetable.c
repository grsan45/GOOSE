#include <stdlib.h>

#include "../include/fs/filetable.h"

int32_t init_ft(file_table *table) {
    table->entries = malloc(sizeof(struct file_bucket) * MAX_FILES); // todo: realloc if we run out of files
    if (table->entries <= 0)
        return -1;

    table->count = 0;

    return 0;
}

int32_t create_entry(file_table *table, pid_t proc, uint32_t fd, fs_node *node, uint8_t access_flags, uint8_t type_flags) {
    if (table->count >= MAX_FILES) return -1;

    uint32_t index = FD_HASH(proc, fd) % MAX_FILES;

    table->entries[index] = (struct file_bucket){.valid = true, .hash = FD_HASH(proc, fd),
            .value = {.access_flags = access_flags, .type_flags = type_flags, .node = node}};

    table->count++;

    return 0;
}

ft_entry find_entry(file_table *table, pid_t proc, uint32_t fd) {
    uint32_t index = FD_HASH(proc, fd) % MAX_FILES;

    struct file_bucket bucket = table->entries[index];
    while (bucket.hash != FD_HASH(proc, fd)) {
        bucket = table->entries[++index];
    }

    if (!bucket.valid)
        return INVALID_ENTRY;

    return bucket.value;
}

int32_t remove_entry(file_table *table, pid_t proc, uint32_t fd) {
    uint32_t index = FD_HASH(proc, fd);
    table->entries[index] = (struct file_bucket){.valid = false};

    table->count--;

    return 0;
}
