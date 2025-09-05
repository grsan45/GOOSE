#ifndef GOOSE_FILEDRIVER_H
#define GOOSE_FILEDRIVER_H

#include <stdint.h>

typedef struct {
    uint32_t (*read)(const char *path, const char *buffer, uint32_t count);
    uint32_t (*write)(const char *path, const char *buffer, uint32_t count);
} file_driver;

#endif //GOOSE_FILEDRIVER_H
