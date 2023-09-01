# Grayson's Odd Operating System Endeavour
### a.k.a GOOSE

---

This project is a way for me to learn more about operating system development.
Loosely guided by the [OSDev wiki](https://wiki.osdev.org) and [Bran's Kernel Guide](http://www.osdever.net/bkerndev/Docs/title.htm).

Written in C & ASM. Don't expect high quality or overly efficient code. 
This project is just for learning and experimenting. Maybe I'll have a "full" user mode OS in a few months.

No custom bootloaders here, using GRUB 2 (multiboot2). Writing a custom bootloader would be 
[very educational](https://www.youtube.com/watch?v=afP127hsceU) but isn't worth the hassle for my
first serious OS project.

---

### Task List:
- [x] Kernel Boots
- [x] GDT, IDT, ISRs
- [x] Output to framebuffer
- [x] Memory Management  
<em>Forever a work in progress, but at least functional for now</em>
- [x] Font rendering
- [x] IRQs
  - [x] Architecture setup
  - [x] PIT
  - [x] Keyboard
- [ ] Custom toolchain **(WIP)**
- [ ] Kernel Modules
<em>Using GRUB here</em>
- [ ] Filesystem
  - [ ] (S)ATA Driver
  - [ ] IDE Driver
  - [ ] FAT32 support (or ext4?)
- [ ] Paging
- [ ] Long mode (64-bit)
  - [ ] SSEn for optimized memory operations
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

### Notes: 
Any code not of my own design (mundane math functions) is 
credited to its "original" author via a comment.

The font is temporarily stored in the `src/` folder until filesystem support is implemented.

---

<sub>Licensed under GPL-3.0</sub>