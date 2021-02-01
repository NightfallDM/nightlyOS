/*
because is in the test and try to do phase,so i may change the code and the 
place where hold the data :-)
*/
.code16
.global start_setup

START_SEG = 0x1000
GDT_BASE_ADDR_seg = 0xf000 //256 gdt_entries
IDT_BASE_ADDR_seg = 0xf400 //follow the GDT end addr
STACK_seg = 0xf000

.text
start_setup:
	movw $START_SEG, %ax
	movw %ax, %es
	movw %ax, %ds
	movb $0x2, %ah
	movb $0x0, %bh
	movw $0xf0a, %dx
	int $0x10

	movw $msg1, %bp
	movw $11, %cx
	movw $0x07, %bx
	xorw %ax, %ax
	movw $0x1301, %ax
	int $0x10
init_gdt:
	//set the stack
	movw $STACK_seg, %ax
	movw $ax, %ss
	movw $0, %sp
	movw %sp, %bp
	lgdt $gdt_start
	//lidt $IDT_BASE_ADDR_seg << 16 + 0x4000
	movw $0x1, %cr0
	ljmp $0x8, $0x0 
	
msg1: .ascii "Hello World"

.org 0xf0000
gdt_start:
	.word 0,0,0,0

	.word 0,0xa,0,0
	.byte 0
	.byte 0x9b
	.byte 0xd0
	.byte 0x0

	.word 0,0xa,0,0
	.byte 0
	.byte 0x92
	.byte 0xd0
	.byte 0x0
	
.org IDT_BASE_ADDR_seg 
idt_start:
	.word 0,0,0,0
	
.org 2045
end: .ascii "END"
