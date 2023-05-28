.section .text

.macro isr, id, errcode=$0x00
.global isr\id
.type isr\id, @function

isr\id :
cli
push \errcode
jmp isr_stub
.endm

isr 0, $0x00
isr 1, $0x01
isr 2, $0x02
isr 3, $0x03
isr 4, $0x04
isr 5, $0x05
isr 6, $0x06
isr 7, $0x07
isr 8, $0x8
isr 9, $0x09
isr 10, $0xA
isr 11, $0xB
isr 12, $0xC
isr 13, $0xD
isr 14, $0xE
isr 15, $0x0F
isr 16, $0x010
isr 17, $0x011
isr 18, $0x012
isr 19, $0x013
isr 20, $0x014
isr 21, $0x015
isr 22, $0x016
isr 23, $0x017
isr 24, $0x018
isr 25, $0x019
isr 26, $0x01A
isr 27, $0x01B
isr 28, $0x01C
isr 29, $0x01D
isr 30, $0x01E
isr 31, $0x01F

.global handle_fault
.type handle_fault, @function

isr_stub:
    pusha               /* push everything we care about to the stack */
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax       /* mov 0x10 (kernel data segment) into ax */
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    mov %esp, %eax      /* put the stack pointer onto eax, */
    push %eax           /* then push that onto the stack so we can access it in handle_fault */

    mov $handle_fault, %eax
    call *%eax

    pop %eax            /* cleanup stack */
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

    add $0x08, %esp
    iret
