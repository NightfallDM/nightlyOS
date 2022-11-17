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
2:	.long pm32_code
	.word (1<<3) 

.code32
.section ".text32", "ax"
pm32_code:
	xorl %eax, %eax
	jmp pm32_code

.p2align 4
gdt:
	SEG_NULLASM			       # null
	SEG_ASM(S_CODE|SC_RD, 0x0, 0xffffffff) # code seg base:0x0 limit:4G
	SEG_ASM(S_DATA|SD_WR, 0x0, 0xffffffff) # data seg base:0x0 limit:4G
	# so,you can see that we just use the flat mode(only one segment)

gdtr_value:
	.word (gdtr_value - gdt -1)	#"-1" need because this ins bigger "1"
	.long gdt			#address gdt
