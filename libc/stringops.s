.global strncmp
.type strncmp, @function

strncmp:
    push %ebp
    mov %esp, %ebp

    mov 8(%ebp), %edi
    mov 12(%ebp), %esi
    mov 16(%ebp), %ecx

    repe
    cmpsb %es:(%edi), (%esi)
    jz passed
    mov $0, %eax
    jmp cleanup
passed:
    mov $1, %eax
cleanup:
    leave
    ret
