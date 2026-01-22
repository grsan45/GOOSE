.section .stack
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .bss, "aw", @nobits
.align 4096
boot_page_directory: # Setup basic paging to allow boot in higher half. Will be overwritten on kernel init
    .skip 4096
boot_page_table1:
    .skip 4096

.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
    # save physical address of boot_page_table1
    movl $(boot_page_table1 - 0xC0000000), %edi

    # mapping start address (addr 0x0)
    movl $0, %esi

    # mapping all 1024 pages. TODO: map vga text buffer & do framebuffer init in kernel
    movl $1024, %ecx
1:
    # map the kernel
    cmpl $_kernel_start, %esi
    jl 2f
    cmpl $(_kernel_end - 0xC0000000), %esi
    jge 3f

    # set flags. This leaves .text and .rodata as writeable, this will be fixed when the table is reconstructed in kernel init
    movl %esi, %edx
    orl $0x003, %edx
    movl %edx, (%edi)
2:
    # move our page pointer by size of page (4096 bytes)
    addl $4096, %esi
    # move our page table entry pointer by size of entry (32 bits)
    addl $4, %edi

    loop 1b
3:
    # map boot_page_table1 to 0x00000000 and 0xC0000000
    movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
    movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4

    # store boot_page_directory in %cr3
    movl $(boot_page_directory - 0xC0000000), %ecx
    movl %ecx, %cr3

    # enable paging
    movl %cr0, %ecx
    orl $0x80010000, %ecx
    movl %ecx, %cr0

    # jump to bootstrap_kernel
    lea bootstrap_kernel, %ecx
    jmp *%ecx

.section .text

bootstrap_kernel:
    # unmap identity mapping
    movl $0, boot_page_directory + 0

    # Reload page table for previous unmap to take affect
    movl %cr3, %ecx
    movl %ecx, %cr3

    # setup stack
    mov $stack_top, %esp

    pushl   $0
    popf

    # push multiboot arguments for kmain
    pushl %ebx
    pushl %eax

    call kmain

    cli
1:
    hlt
    jmp 1b

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
