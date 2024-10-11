#ifndef GOOSE_SYSCALLS_H
#define GOOSE_SYSCALLS_H

#include "idt.h"

typedef struct {
    uint32_t ebx, ecx, edx, esi, edi, ebp;
} syscall_args;

int sys_read(syscall_args *args);
int sys_write(syscall_args *args);
int sys_open(syscall_args *args);
int sys_close(syscall_args *args);

static int (*sys_calls[16])(syscall_args *) = {
        sys_read, sys_write, sys_open, sys_close,
        [4 ... 15] = 0
};

void handle_syscall();
void setup_syscall_handler();

#endif //GOOSE_SYSCALLS_H
