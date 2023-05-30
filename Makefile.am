bin_PROGRAMS=goose
goose_SOURCES = src/boot/multiboot.s src/boot/boot.s src/arch/isrs.s src/arch/gdt.c src/arch/idt.c src/display/framebuffer.c src/string.c src/arch/memmgt.c src/io/serial.c src/kernel.c
goose_LDFLAGS = @PACKAGE_LDFLAGS@
goose_LDADD = @PACKAGE_LDADD@
AM_CPPFLAGS = @PACKAGE_CFLAGS@
AM_CCASFLAGS = @PACKAGE_ASFLAGS@