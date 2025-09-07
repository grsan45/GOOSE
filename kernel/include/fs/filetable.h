#ifndef GOOSE_FDT_H
#define GOOSE_FDT_H

#include <stdint.h>
#include <unistd.h>
#include <filedriver.h>

#include <math.h>

#define FREAD 0b0001
#define FWRITE 0b0010
#define FAPPEND 0b0100

#define TYPE_FILE 0b0001
#define TYPE_DIRECTORY 0b0010
#define TYPE_DEVICE 0b0100

#define MAX_FILES 4096
#define MAX_NODES 4096

typedef struct {
    char* name;
    uint8_t type_flags;
    file_driver *driver; // todo: better way
} fs_node;

typedef struct s_ft_entry {
    uint8_t access_flags;
    uint8_t type_flags;
    fs_node *node;
} ft_entry;

#define INVALID_ENTRY (ft_entry){}

typedef struct s_file_table {
    struct file_bucket {
        bool valid;
        uint32_t hash;
        ft_entry value;
    } *entries;
    uint32_t count;
} file_table;

extern file_table main_ft;
#define FILE_TABLE main_ft

int32_t init_ft(file_table *table);
ft_entry find_entry(file_table *table, pid_t proc, uint32_t fd);
int32_t create_entry(file_table *table, pid_t proc, uint32_t fd, fs_node *node,
                     uint8_t access_flags, uint8_t type_flags);
int32_t remove_entry(file_table *table, pid_t proc, uint32_t fd);

#define FD_HASH(proc, fd) HASH(HASH((proc)) + HASH((fd)))

#endif //GOOSE_FDT_H
