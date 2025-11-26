#include <arch/syscalls.h>
#include <io/serial.h>

#include "stddef.h"
#include "../include/fs/filetable.h"

void handle_syscall(isr_stacktrace *args) {
    syscall_args s_args = {args->ebx, args->ecx, args->edx, args->esi, args->edi, args->ebp};
    (sys_calls[args->eax])(&s_args);
}

void setup_syscall_handler() {
    set_irq_handler(96, handle_syscall);
}

int sys_read(syscall_args *args) {return 0;}

int sys_write(syscall_args *args) {
    uint32_t *arg_arr = (uint32_t*)args->ebx;

    uint32_t fd = arg_arr[0];
    char* str = (char*)arg_arr[1];

    if (fd == 0)
    {
        serial_printf(COM1, "%s", str);
        return 0;
    }

    const ft_entry entry = find_entry(FILE_TABLE, 0, fd);
    const fs_node *node = entry.node;

    node->driver->write(NULL, str, 0);

    return 0;
}

int sys_open(syscall_args *args) {return 0;}
int sys_close(syscall_args *args) {return 0;}
