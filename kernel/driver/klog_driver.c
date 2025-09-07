#include "../include/driver/klog_driver.h"

uint32_t klog_write(const char *path, const char *buffer, uint32_t count) {
    puts(buffer);

    return count;
}

uint32_t klog_read(const char *path, const char *buffer, uint32_t count) {
    return 0;
}