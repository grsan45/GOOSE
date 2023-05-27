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

    pushl   $0
    popf

    pushl %ebx
    pushl %eax

    call kmain

    cli
1:
    hlt
    jmp 1

.size _start, . - _start

.global gdt_flush
.type gdt_flush, @function

gdt_flush:
    lgdt (gdtr)
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    jmp $0x08,$flush2
flush2:
    ret

.global idt_load
.type idt_load, @function

idt_load:
    lidt (idtr)
    ret
