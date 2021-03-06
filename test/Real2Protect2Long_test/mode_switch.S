# Test the switch between three mode:
#	--real mode  --protected mode  --IA32e mode(Long mode)
# Reference from Intel Software Developer's Manual
# Volume 3: System Programming Guide
#
# Also want to use gdb to debug the bootloader.

#include "setup.h"
.code16
.global start

# The Order(real mode 2 protected mode)(without PAGE)
# 1. Disable the interrupt
# 2. Enable the A20 line
# 3. Load GDT
# 4. Setup CR0 to enable the PE
# 5. Far JMP or CALL instruction after the MOV CR0 to refresh the segment reg
start:
	xorw %ax, %ax
	movw %ax, %es
	movw %ax, %ds
	movw %ax, %ss

clear_screen:
	movw $0x600, %ax
	xorw %cx, %cx
	int $0x10
	

enable_A20:
	inb $0x92, %al
	orb $0x2, %al
	outb %al, $0x92

load_4_sectors:
	movw $0x7e00, %bx
	movw $0x204, %ax
	movw $0x1, %cx
	movw $0x80, %dx
	int $0x13
	cli
	

switch_protected:
	lgdt end_GDT
	movl %cr0, %eax
	orl $0x1, %eax
	movl %eax, %cr0
	ljmp $(1<<3), $print

.code32
print:
	jmp print

// set GDT
.p2align 2
set_GDT_entries:
	.quad 0
	#GDTE(0, 0xfffff, KERNEL_C)
	#GDTE(0, 0xfffff, KERNEL_D)
	#GDTE(0, 0xfffff, USER_C)
	#GDTE(0, 0xfffff, USER_D)
	.long 0xff
	.byte 0x0
	.byte 0x9b
	.byte 0xcf
	.byte 0x0
	
end_GDT:
	.word (end_GDT - set_GDT_entries - 1) # limit of all GDTE
	.long set_GDT_entries # GDT BASE address



.org 510
end:
	.word 0xaa55
