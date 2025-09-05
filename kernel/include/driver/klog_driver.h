#ifndef GOOSE_KLOG_DRIVER_H
#define GOOSE_KLOG_DRIVER_H

#include <filedriver.h>
#include <display/framebuffer.h>

uint32_t read(const char *path, const char *buffer, uint32_t count);
uint32_t write(const char *path, const char *buffer, uint32_t count);

#endif //GOOSE_KLOG_DRIVER_H
