#include "../include/driver/klog_driver.h"

uint32_t write(const char *path, const char *buffer, uint32_t count) {
    puts(buffer);

    return count;
}