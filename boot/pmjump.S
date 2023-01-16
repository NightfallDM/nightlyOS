#include "asm.h"

.code16
.text

.global protected_switch
protected_switch:
	xorl %ebx, %ebx
	movw %cs, %bx
	shll $4, %ebx
	addl %ebx, 2f
	addl %ebx, (gdtr_value+2)
	jmp 1f
1:
	lgdt gdtr_value
	cli
	movl %cr0, %eax
	orl $0x1, %eax
	movl %eax, %cr0

	# xorl %eax, %eax
	# xorl %ebx, %ebx
	# movl (__boot_param+5), %ebx
	# movl 4(%ebx), %eax
	# ljmp $(1<<3), *%eax
	.byte 0x66, 0xea
#2:	.long pm32_code
2:	.long lba28_load_header
	.word (1<<3) 

.code32
.section ".text32", "ax"
#pm32_code:
	#xorl %eax, %eax
	#jmp pm32_code
lba28_load_header:
    xorw %ax, %ax
    xorw %dx, %dx
    movw $0x1f2, %dx
    movw $600, %ax
    outw %ax, %dx

    movw $0x5, %ax
    movw $0x1f3, %dx
    outw %ax, %dx

    xorw %ax, %ax
    movw $0x1f4, %dx
    outw %ax, %dx

    movw $0x1f5, %dx
    outw %ax, %dx

    movw $0xe0, %ax
    movw $0x1f6, %dx
    outw %ax, %dx

    movw $0x20, %ax
    movw $0x1f7, %dx
    outw %ax, %dx

check_disk_status:
    nop
    inw %dx, %ax
    andb $0x88, %al
    cmpb $0x8, %al
    jnz check_disk_status

    movl $600, %ecx
    movl %ecx, %eax
    movl $256, %edx
    mull %edx
    movl %eax, %ecx
    #movw $LOAD_SEG, %bx
    #movw %bx, %ds
    movl $0x10000, %ebx
    movw $0x1f0, %dx

read_data:
    inw %dx, %ax
    outw %ax, $0x80
    movw %ax, (%ebx)
    addl $2, %ebx

    movw $0x1f1, %dx
    inw %dx, %ax
    cmpb $0, %al
    jnz spin
    movw $0x1f0, %dx
    loop read_data
	jmp 0x10000

spin:
	jmp spin
.p2align 4
gdt:
	SEG_NULLASM			       # null
	SEG_ASM(S_CODE|SC_RD, 0x0, 0xffffffff) # code seg base:0x0 limit:4G
	SEG_ASM(S_DATA|SD_WR, 0x0, 0xffffffff) # data seg base:0x0 limit:4G
	# so,you can see that we just use the flat mode(only one segment)

gdtr_value:
	.word (gdtr_value - gdt -1)	#"-1" need because this ins bigger "1"
	.long gdt			#address gdt
