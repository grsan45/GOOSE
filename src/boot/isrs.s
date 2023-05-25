.section .text

.global isr0
.type isr0, @function

isr0:
    cli
    push $0x0

    jmp isr_stub

.global isr8
.type isr8, @function

isr8:
    cli
    push $0x8

    jmp isr_stub

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
