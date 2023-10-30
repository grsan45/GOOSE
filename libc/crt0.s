# https://wiki.osdev.org/Creating_a_C_Library

.section .text

.global _start
_start:
	# Set up end of the stack frame linked list.
	mov $0, %ebp
	pushl %ebp # eip=0
	pushl %ebp # ebp=0
	mov %esp, %ebp

	# We need those in a moment when we call main.
	pushl %esi
	pushl %edi

	# Prepare signals, memory allocation, stdio and such.
	# call initialize_standard_library #TODO

	# Run the global constructors.
	call _init

	# Restore argc and argv.
	popl %edi
	popl %esi

	# Run main
	call main

	# Terminate the process with the exit code.
	mov %eax, %edi
	call exit
.size _start, . - _start
