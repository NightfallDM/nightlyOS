//date: 2020/08/18
//author: night
//email: 1335715927@qq.com

//we are now in the real mode so :-)
.code16
.global begbss, begtext, begdata, start_boot

//set the useful msg
START_SEG = 0x07c0
LOAD_SEG = 0x1000


.text
start_boot:
	movw $START_SEG, %ax
	movw %ax, %es
	movw %ax, %ds
	jmp start
	msg:
		.ascii "Hello Night!!!\n"
	// test print string by using the bios call
	
	//set the location of cursor
start:
	movb $0x2, %ah
	movb $0x0, %bh
	movw $0xa0a, %dx
	int $0x10

	//put string
	movw $msg, %bp
	movw $15, %cx
	movw $0x07, %bx
	xorw %ax, %ax
	movw $0x1301, %ax
	int $0x10

//load 4 sectors at 0x10000
load_4_sectors:
	movw $0x204, %ax
	xorw %cx, %cx
	movb $0x2, %cl
	movw $LOAD_SEG, %dx
	movw %dx, %es
	movw $0x0, %bx
	//xorw %dx, %dx
	movw $0x80, %dx
	int $0x13
	cmpb $0x0, %ah
	jne print_error_msg
	movw $LOAD_SEG, %ax
	movw %ax, %es
	movw %ax, %ds
	ljmp $LOAD_SEG, $0x0	

print_error_msg:
	xorw %ax, %ax
	movb $0x2, %ah
	movb $0x0, %bh
	movw $0xb0a, %dx
	int $0x10

	movw $START_SEG, %ax
	movw %ax, %es
	movw $Error_msg, %bp
	movw $32, %cx
	movw $0x07, %bx
	xorw %ax, %ax
	movw $0x1301, %ax
	int $0x10

Error_msg: .ascii "failed to read the sectors, exit"
.org 510
end_signal :
	.word 0xaa55

		

