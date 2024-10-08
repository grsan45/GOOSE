#include <arch/syscall.h>
#include <arch/idt.h>
#include <io/serial.h>

void handle_syscall() {
    serial_printf(COM1, "syscall interrupt triggered");
}

void setup_syscall_handler() {
    set_irq_handler(96, handle_syscall);
}