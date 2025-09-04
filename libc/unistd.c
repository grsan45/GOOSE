#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>

int write(uint32_t fd, char* buf) {
    uint32_t args[2] = {fd, (uint32_t) buf};
    __syscall((uint32_t) SYS_WRITE, args);
    return strlen(buf);
}