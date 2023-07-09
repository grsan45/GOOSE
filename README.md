# Grayson's Odd Operating System Endeavour
### a.k.a GOOSE

---

This project is a way for me to learn more about operating system development.
Loosely guided by the [OSDev wiki](https://wiki.osdev.org) and [Bran's Kernel Guide](http://www.osdever.net/bkerndev/Docs/title.htm).

Written in C & ASM. Don't expect high quality or overly efficient code. 
This project is just for learning and experimenting. Maybe I'll have a "full" user mode OS in a couple of months.

No custom bootloaders here, using GRUB 2 (multiboot2). As educational as writing a custom bootloader is, I didn't want
to go through the hassle for my first serious toy OS.
---

### Task List:
- [x] Kernel Boots
- [x] GDT, IDT, ISRs
- [x] Output to framebuffer
- [ ] Memory Management ***(WIP)***
- [ ] Font rendering
- [ ] IRQs
- [ ] Paging
- [ ] 64-bit support
- [ ] FS support
- [ ] User Mode!
---
### Want to build?
This project can only be built on linux or WSL. You'll need autotools and qemu installed.

You'll need an i686-elf cross-compiler, use this guide to create one: https://wiki.osdev.org/GCC_Cross-Compiler.
I recommend creating a folder in the root directory of the project called `i686-elf-cc` and setting the `$PREFIX` variable
described in the above guide to that directory. Make sure you have the `$TARGET` variable set to `i686-elf`.

To generate build files, run `./autogen.sh` from the project's root directory.
After that, you can run `make all` from the build directory to create the OS binary.
*Note: if you installed the i686-elf CC in a different directory than recommended above,
you'll need to remove the first if statement from autogen.sh and update the autotools configuration files.*

To create the ISO image, run `./create_iso.sh` inside the `tools` directory.

Use `qemu-system-i386 -serial stdio --cdrom goose.iso` to run the OS.

---
<sub>Licensed under GPLv3.0</sub>