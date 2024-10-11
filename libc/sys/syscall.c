#include <sys/syscall.h>

uint32_t syscall(uint32_t code, uint32_t args, ...) {
    va_list kargs;
    va_start(kargs, args);

    uint32_t s_args[MAX_ARGS] = {0};

    for (int i = 0; i < MAX_ARGS && args != 0; i++, args = va_arg(kargs, uint32_t)) {
        s_args[i] = args;
    }

    __syscall(code, s_args);

    return 0;
}