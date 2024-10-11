#include <arch/syscalls.h>
#include <io/serial.h>
#include <display/framebuffer.h>

void handle_syscall(isr_stacktrace *args) {
    serial_printf(COM1, "syscall interrupt triggered: %d\n", args->eax);

    syscall_args s_args = {args->ebx, args->ecx, args->edx, args->esi, args->edi, args->ebp};
    (sys_calls[args->eax])(&s_args);
}

void setup_syscall_handler() {
    set_irq_handler(96, handle_syscall);
}

int sys_read(syscall_args *args) {return 0;}

int sys_write(syscall_args *args) {
    uint32_t *argArr = (uint32_t *) args->ebx;

    // ignoring file descriptor for now
//    FILE *fp = (FILE *) argArr[0];
    char *str = (char *) argArr[1];

    serial_printf(COM1, "sys_write, fp: %d, str: %s\n", argArr[0], str);
    printf("%s", str);

    return 0;
}

int sys_open(syscall_args *args) {return 0;}
int sys_close(syscall_args *args) {return 0;}
