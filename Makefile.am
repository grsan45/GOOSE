AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS=goose
goose_SOURCES = src/boot/multiboot.s src/boot/boot.s src/arch/isrs.s src/math.c src/arch/gdt.c src/arch/idt.c src/display/framebuffer.c src/display/psf_font.c src/string.c src/arch/memmgt.c src/io/ioutils.c src/io/serial.c src/io/timer.c src/kernel.c
goose_LDFLAGS = @PACKAGE_LDFLAGS@
goose_LDADD = @PACKAGE_LDADD@
AM_CPPFLAGS = @PACKAGE_CFLAGS@
AM_CCASFLAGS = @PACKAGE_ASFLAGS@