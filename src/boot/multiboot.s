/* multiboot header */

.set MAGIC, 0xe85250d6
.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set CHECKSUM, 0x100000000 - (0xe85250d6 + 0 + (multiboot_start - multiboot_end))

.section .multiboot
multiboot_start:
.align 4
.long MAGIC
.long 0
.long (multiboot_start - multiboot_end)

.long CHECKSUM

.word 0
.word FLAGS
.long 8
multiboot_end:
