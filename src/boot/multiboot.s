/* multiboot header */

.set MAGIC, 0xe85250d6
.set ALIGN, 1<<0
.set MEMINFO, 1<<1
.set FLAGS, ALIGN | MEMINFO
.set CHECKSUM, 0x100000000 - (0xe85250d6 + 0 + (multiboot_start - multiboot_end))

.section .multiboot
multiboot_start:
.align 8
.long MAGIC
.long 0
.long (multiboot_start - multiboot_end)

.long CHECKSUM

info_request_tag_start:
.word 1
.word 0
.long info_request_tag_end - info_request_tag_start
.long 6
.long 8
info_request_tag_end:

framebuffer_tag_start:
.word 5
.word 0
.long framebuffer_tag_end - framebuffer_tag_start
.long 1280
.long 720
.long 24
framebuffer_tag_end:

.word 0
.word FLAGS
.long 0
multiboot_end:
