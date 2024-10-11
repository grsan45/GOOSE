.global __syscall
.type __syscall @function

__syscall:
    push %ebp
    mov %esp, %ebp

    mov 8(%ebp), %eax

    int $0x80
    pop %ebp
    ret
