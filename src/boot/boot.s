.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.type _start, @function

_start:
    mov $stack_top, %esp

    call kmain

    cli
1:
    hlt
    jmp 1b

.size _start, . - _start

.global gdt_flush
.type gdt_flush, @function

gdt_flush:
    lgdt (gdtr)
    mov %ax, 0x10
    mov %ds, %ax
    mov %es, %ax
    mov %fs, %ax
    mov %gs, %ax
    mov %ss, %ax
    jmp $0x08,$flush2
flush2:
    ret
