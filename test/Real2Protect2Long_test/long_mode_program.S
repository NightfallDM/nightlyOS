.code64
.global real_long_mode
real_long_mode:
	cli
	movw $(1<<3), %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	movl $0xb8000, %edi
	movq $0x1f201f201f201f20, %rax
	movl $500, %ecx
	rep stosq
	hlt
	
