# date: 2020/08/26

/*
This is the entry point in the kernel,it will link with kernel
So, it means that the entry.S are in the kernel,We can refer the 
symbol table that define in the kernel.

After enable the pageing, we will jump to the "main.c".
*/

.global _start:
_start:
	jmp entry

.global entry
entry:
	# PSE
	#Turn on page size extension for 4Mbyte pages
	movl %cr4, %eax
	orl $0x10, %eax
	movl %eax, %cr0

	# Set page directory in cr3
	movl $(V2P_WO(entrypgdir)), %eax # define in the kernel later
	movl %eax, %cr3

	#Turn on paging
	movl %cr0, %eax
	orl $0x80010000, %eax # CR0_PG|CR0_WP
	movl %eax, %cr0

	#Set up the stack point.
	movl $(stack + 4096), %esp
	
	# Jump to main(), and switch to executing at
 	# high addresses. The indirect call is needed because
 	# the assembler produces a PC-relative instruction
  	# for a direct jump.
  	mov $main, %eax
  	jmp *%eax # !!! may dereference explicit

.comm stack, 4096
# it mean alloc 4096 Byte space for symbol "stack"
# may just like use ".long" to alloc 4096 Byte apace
# Note that we add 4096 to the stack addr because the 
# stack grow with the addr desc
