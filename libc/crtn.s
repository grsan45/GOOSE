# https://wiki.osdev.org/Creating_a_C_Library

.section .init
   /* gcc will nicely put the contents of crtend.o's .init section here. */
   popl %ebp
   ret

.section .fini
   /* gcc will nicely put the contents of crtend.o's .fini section here. */
   popl %ebp
   ret

