# Grayson's Odd Operating System Endeavour
### a.k.a GOOSE

---

This project is a way for me to learn more about operating system development.
Loosely guided by the [OSDev wiki](https://wiki.osdev.org) and [Bran's Kernel Guide](http://www.osdever.net/bkerndev/Docs/title.htm).

Written in C & ASM. Don't expect high quality or overly efficient code. 
This project is just for learning and experimenting. Maybe I'll have a "full" user mode OS in a couple of months.

No custom bootloaders here, using GRUB 2 (multiboot2). As educational as writing a custom bootloader is, I didn't want
to go through the hassle for my first serious toy OS.

Needs an i686-elf cross-compiler present in the project directory to compile.

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

<sub>Licensed under GPLv3.0</sub>