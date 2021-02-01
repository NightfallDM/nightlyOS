# load "setup" part and fill some "header" field

.code16
.global loader_start

loader_start:
	movw $0x7c0, %ax
	movw %ax, %es
	movw $0x20a, %ax # read 10 sectors
	movw $0x2, %cx
	movw $0x80, %dx
	movw $0x200, %bx
	int $0x13
	jc die
	ljmp $0x7e0, $0x0 # ljmp to setup code

die:
	hlt
	jmp die
