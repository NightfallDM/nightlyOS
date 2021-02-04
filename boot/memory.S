.code16
.global detect_memory

detect_memory:
	movw $0x800, %ax
	movw %ax, %di
	movl $0xe820, %eax
	movl $0x534d4150, %edx
	xorl %ebx, %ebx
	movl $20, %ecx
	int $0x15
next:
	jc RT
	# __boot_parm defined in "main.c"
	addb $1, (__boot_param)
	movl $0x534d4150, %edx
	testl %ebx, %ebx
	je RT
	cmp %edx, %eax
	jne RT
	addw $20, %di
	int $0x15
	jmp next


RT:
	ret

