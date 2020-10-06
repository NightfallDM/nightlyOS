.code16
.global main

main:
	xorw %ax, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
A20_on:
	inb $0x92, %al
	orb $0x2, %al
	outb %al, $0x92

#read_4_sectors:
#	movw $0x0204, %ax
#	movw $1, %cx
#	movw $0x80, %dx
#	movw $0x7e00, %bx
#	int $0x13
	cli
switch1:
	lgdt GDTR_V
	movl %cr0, %eax
	orl $0x1, %eax
	movl %eax, %cr0
	ljmp $0x8, $switch2

# never reach this place
spin:
	jmp spin

.code32
switch2:
	movl $(1<<20), %eax
	movl %eax, %cr3

	movb $0x23, (0x10000)
	movw $0x110, (0x10001)
	movw $0xc3, (0x11000)
	movl $0xa000000, (0x11003)
	
	movl %cr4, %eax
	orl $(1<<5), %eax
	movl %eax, %cr4	

	movl $0xc0000080, %eax
	rdmsr
	orl $(1<<8), %eax
	wrmsr

	movl %cr0, %eax
	orl $(1<<31), %eax
	movl %eax, %cr0
	
enter_64:
#	movl $0x100000, %eax
#	movl %eax, %esp
	lgdt GDTR_64V
	ljmp $(1<<3), $mode_64
.code64
mode_64:
	jmp mode_64

.p2align 2
gdt32:
	.word 0, 0; .byte 0, 0, 0, 0
	.word 0xffff # limit 15:0
	.word 0x0 # base 15:0
	.byte 0x0 # base 23:16
	.byte 0x9b # access byte
	.byte 0xcf # flags | limit 19:16
	.byte 0x0 #base 31:24
GDTR_V:
	.word (GDTR_V - gdt32 -1) # gdt limit
	.long gdt32 # liner base address

.p2align 2
gdt64:
	.word 0, 0; .byte 0, 0, 0, 0
	.word 0x0
	.word 0x0
	.byte 0x0
	.byte 0x9b
	.byte 0xaf
	.byte 0x0
	
	.word 0x0
	.word 0x0
	.byte 0x0
	.byte 0x93
	.byte 0x0
	.byte 0x0
GDTR_64V:
	.word (GDTR_64V - gdt64 -1)
	.long gdt64

.org 510
END:
	.word 0xaa55
