# generated from kernel/arch/isrs.s.json
.section .text

.macro isr, id, errcode=$0x00
.global isr\id
.type isr\id, @function

isr\id :
cli
push \errcode
jmp isr_stub
.endm

.macro irq, id, index=$0x020
.global irq\id
.type irq\id, @function

irq\id :
cli
push 0x00
push \index
jmp irq_stub
.endm

/* isrs */
isr 0, $0x0
isr 1, $0x1
isr 2, $0x2
isr 3, $0x3
isr 4, $0x4
isr 5, $0x5
isr 6, $0x6
isr 7, $0x7
isr 8, $0x8
isr 9, $0x9
isr 10, $0xA
isr 11, $0xB
isr 12, $0xC
isr 13, $0xD
isr 14, $0xE
isr 15, $0xF
isr 16, $0x10
isr 17, $0x11
isr 18, $0x12
isr 19, $0x13
isr 20, $0x14
isr 21, $0x15
isr 22, $0x16
isr 23, $0x17
isr 24, $0x18
isr 25, $0x19
isr 26, $0x1A
isr 27, $0x1B
isr 28, $0x1C
isr 29, $0x1D
isr 30, $0x1E
isr 31, $0x1F
/* isqs */
irq 0, $0x20
irq 1, $0x21
irq 2, $0x22
irq 3, $0x23
irq 4, $0x24
irq 5, $0x25
irq 6, $0x26
irq 7, $0x27
irq 8, $0x28
irq 9, $0x29
irq 10, $0x2A
irq 11, $0x2B
irq 12, $0x2C
irq 13, $0x2D
irq 14, $0x2E
irq 15, $0x2F
irq 16, $0x30
irq 17, $0x31
irq 18, $0x32
irq 19, $0x33
irq 20, $0x34
irq 21, $0x35
irq 22, $0x36
irq 23, $0x37
irq 24, $0x38
irq 25, $0x39
irq 26, $0x3A
irq 27, $0x3B
irq 28, $0x3C
irq 29, $0x3D
irq 30, $0x3E
irq 31, $0x3F
irq 32, $0x40
irq 33, $0x41
irq 34, $0x42
irq 35, $0x43
irq 36, $0x44
irq 37, $0x45
irq 38, $0x46
irq 39, $0x47
irq 40, $0x48
irq 41, $0x49
irq 42, $0x4A
irq 43, $0x4B
irq 44, $0x4C
irq 45, $0x4D
irq 46, $0x4E
irq 47, $0x4F
irq 48, $0x50
irq 49, $0x51
irq 50, $0x52
irq 51, $0x53
irq 52, $0x54
irq 53, $0x55
irq 54, $0x56
irq 55, $0x57
irq 56, $0x58
irq 57, $0x59
irq 58, $0x5A
irq 59, $0x5B
irq 60, $0x5C
irq 61, $0x5D
irq 62, $0x5E
irq 63, $0x5F
irq 64, $0x60
irq 65, $0x61
irq 66, $0x62
irq 67, $0x63
irq 68, $0x64
irq 69, $0x65
irq 70, $0x66
irq 71, $0x67
irq 72, $0x68
irq 73, $0x69
irq 74, $0x6A
irq 75, $0x6B
irq 76, $0x6C
irq 77, $0x6D
irq 78, $0x6E
irq 79, $0x6F
irq 80, $0x70
irq 81, $0x71
irq 82, $0x72
irq 83, $0x73
irq 84, $0x74
irq 85, $0x75
irq 86, $0x76
irq 87, $0x77
irq 88, $0x78
irq 89, $0x79
irq 90, $0x7A
irq 91, $0x7B
irq 92, $0x7C
irq 93, $0x7D
irq 94, $0x7E
irq 95, $0x7F
irq 96, $0x80
irq 97, $0x81
irq 98, $0x82
irq 99, $0x83
irq 100, $0x84
irq 101, $0x85
irq 102, $0x86
irq 103, $0x87
irq 104, $0x88
irq 105, $0x89
irq 106, $0x8A
irq 107, $0x8B
irq 108, $0x8C
irq 109, $0x8D
irq 110, $0x8E
irq 111, $0x8F
irq 112, $0x90
irq 113, $0x91
irq 114, $0x92
irq 115, $0x93
irq 116, $0x94
irq 117, $0x95
irq 118, $0x96
irq 119, $0x97
irq 120, $0x98
irq 121, $0x99
irq 122, $0x9A
irq 123, $0x9B
irq 124, $0x9C
irq 125, $0x9D
irq 126, $0x9E
irq 127, $0x9F
irq 128, $0xA0
irq 129, $0xA1
irq 130, $0xA2
irq 131, $0xA3
irq 132, $0xA4
irq 133, $0xA5
irq 134, $0xA6
irq 135, $0xA7
irq 136, $0xA8
irq 137, $0xA9
irq 138, $0xAA
irq 139, $0xAB
irq 140, $0xAC
irq 141, $0xAD
irq 142, $0xAE
irq 143, $0xAF
irq 144, $0xB0
irq 145, $0xB1
irq 146, $0xB2
irq 147, $0xB3
irq 148, $0xB4
irq 149, $0xB5
irq 150, $0xB6
irq 151, $0xB7
irq 152, $0xB8
irq 153, $0xB9
irq 154, $0xBA
irq 155, $0xBB
irq 156, $0xBC
irq 157, $0xBD
irq 158, $0xBE
irq 159, $0xBF
irq 160, $0xC0
irq 161, $0xC1
irq 162, $0xC2
irq 163, $0xC3
irq 164, $0xC4
irq 165, $0xC5
irq 166, $0xC6
irq 167, $0xC7
irq 168, $0xC8
irq 169, $0xC9
irq 170, $0xCA
irq 171, $0xCB
irq 172, $0xCC
irq 173, $0xCD
irq 174, $0xCE
irq 175, $0xCF
irq 176, $0xD0
irq 177, $0xD1
irq 178, $0xD2
irq 179, $0xD3
irq 180, $0xD4
irq 181, $0xD5
irq 182, $0xD6
irq 183, $0xD7
irq 184, $0xD8
irq 185, $0xD9
irq 186, $0xDA
irq 187, $0xDB
irq 188, $0xDC
irq 189, $0xDD
irq 190, $0xDE
irq 191, $0xDF
irq 192, $0xE0
irq 193, $0xE1
irq 194, $0xE2
irq 195, $0xE3
irq 196, $0xE4
irq 197, $0xE5
irq 198, $0xE6
irq 199, $0xE7
irq 200, $0xE8
irq 201, $0xE9
irq 202, $0xEA
irq 203, $0xEB
irq 204, $0xEC
irq 205, $0xED
irq 206, $0xEE
irq 207, $0xEF
irq 208, $0xF0
irq 209, $0xF1
irq 210, $0xF2
irq 211, $0xF3
irq 212, $0xF4
irq 213, $0xF5
irq 214, $0xF6
irq 215, $0xF7
irq 216, $0xF8
irq 217, $0xF9
irq 218, $0xFA
irq 219, $0xFB
irq 220, $0xFC
irq 221, $0xFD
irq 222, $0xFE
irq 223, $0xFF
.global handle_fault
.type handle_fault, @function

.global handle_irq
.type handle_irq, @function

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

irq_stub:               /* this is the exact same as isr_stub, but calls a different handler method */
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

    mov $handle_irq, %eax
    call *%eax

    pop %eax            /* cleanup stack */
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

    add $0x08, %esp
    iret

