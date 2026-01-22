ENTRY(_start)

VIRTUAL_START = 0xC0000000;

SECTIONS {
    . = 1M;

    _kernel_start = .;
    .multiboot.data :
    {
        *(.multiboot.data)
    }

    .multiboot.text :
    {
        *(.multiboot.text)
    }

    /* move to 3GB */
    . += VIRTUAL_START;

    /* code section */
    .text ALIGN (4K) : AT (ADDR (.text) - VIRTUAL_START)
    {
        *(.text)
    }

    /* readonly data */
    .rodata ALIGN (4K) : AT (ADDR (.rodata) - VIRTUAL_START)
	{
		*(.rodata)
	}

	/* r/w data */
	.data ALIGN (4K) : AT (ADDR (.data) - VIRTUAL_START)
	{
		*(.data)
	}

	.bss ALIGN (4K) : AT (ADDR (.bss) - VIRTUAL_START)
	{
		*(COMMON)
		*(.bss)
        *(.stack)
    }

    _kernel_end = .;
}