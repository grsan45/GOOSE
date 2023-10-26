# https://wiki.osdev.org/Creating_a_C_Library

.section .init
.global _init
_init:
   push %ebp
   movq %esp, %ebp
   /* gcc will nicely put the contents of crtbegin.o's .init section here. */

.section .fini
.global _fini
_fini:
   push %ebp
   movq %esp, %ebp
   /* gcc will nicely put the contents of crtbegin.o's .fini section here. */