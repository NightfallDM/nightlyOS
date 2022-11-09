.code16
.global _start
_start:
	jmp xxx
	xorw %ax, %ax
	movw %ax, %ds
xxx:
	jmp xxx
