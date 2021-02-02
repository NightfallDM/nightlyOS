.code16
.global _start
_start:
	movw $0x4f02, %ax
	movw $0x107, %bx
	int $0x10
	jmp _loop

_loop:
	jmp _loop
.org 510
end:
	.word 0xAA55
