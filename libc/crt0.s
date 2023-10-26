# https://wiki.osdev.org/Creating_a_C_Library

.section .text

.global _start
_start:
	# Set up end of the stack frame linked list.
	movq $0, %ebp
	pushq %ebp # eip=0
	pushq %ebp # ebp=0
	movq %esp, %ebp

	# We need those in a moment when we call main.
	pushq %esi
	pushq %edi

	# Prepare signals, memory allocation, stdio and such.
	# call initialize_standard_library #TODO

	# Run the global constructors.
	call _init

	# Restore argc and argv.
	popq %edi
	popq %esi

	# Run main
	call main

	# Terminate the process with the exit code.
	movl %eax, %edi
	call exit
.size _start, . - _start