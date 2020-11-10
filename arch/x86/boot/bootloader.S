.code16
.section ".bltext", "ax"
.global start_of_bootloader
start_of_bootloader:
	# load rest 20 sectors that store stage2 of bootloader
	movw $0x0214, %ax
	movw $0x2, %cx
	movw $0x80, %dx
	movw $0x7e0, %bx
	movw %bx, %es
	xorw %bx, %bx
	int $0x13
	jc error
enable_A20:
	inb $0x92, %al
	orb $0x2, %al
	outb %al, $0x92
get_mm_map:
	movw $0x50, %ax
	movw %ax, %es
	movw $0x4, %di
e820:	movl $0xe820, %eax
	movl $0x534d4150, %edx
	int $0x15
	jc error
	incw (0x500)
	addw $20, %di
	cmpl $0, %ebx
	jne e820
setup_stack:
	xorw %ax, %ax
	movw %ax, %sp
	movw %ax, %bp
	movw $0x9fc0, %ax
	movw %ax, %ss
	jmp 0x7e00
	

error:
	hlt
	
.org 510
end:
	.word 0xaa55
