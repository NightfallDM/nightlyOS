// this is a very simple load just load the 4 sector after this sector;
// fill some field in the headers and then jump to the '_start' in the 'header.S'

//we are now in the real mode so :-)
//.code16

//set the useful msg
START_SEG = 0x07c0
LOAD_SEG = 0x1000
.code16
.section ".bltext", "ax"
.global start_boot

start_boot:
	ljmp $START_SEG, $start
	//set the location of cursor

start:
	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	xorw %sp, %sp
	sti
	cld
	movw $msgl, %si

	/*
	movb $0x2, %ah
	movb $0x0, %bh
	movw $0xa00, %dx
	int $0x10
	*/

	//put string
put_msg_loop:
	lodsb
	andb %al, %al
	jz load_4_sectors
	movb $0xe, %ah
	movw $0x700, %bx
	int $0x10
	jmp put_msg_loop

//load 4 sectors at 0x10000
load_4_sectors:
	movw $0x204, %ax
	xorw %cx, %cx
	movb $0x2, %cl
	movw $LOAD_SEG, %dx
	movw %dx, %es
	movw $0x0, %bx
	xorw %dx, %dx
	movw $0x80, %dx
	int $0x13
	// cmpb $0x0, %ah
	jc test
	jnc test
	jne print_error_msg_xxx
	movw $LOAD_SEG, %ax
	movw %ax, %es
	movw %ax, %ds

	# fill the hdr field
test:
	movw $LOAD_SEG, %bx
	movb $LOAD_SEG & 0xff, 498(%bx)
	movb $LOAD_SEG >> 8, 499(%bx)
	ljmp $LOAD_SEG, $512

print_error_msg_xxx:
	movw $Error_msg, %si
print_error_msg:
	lodsb
	andb %al, %al
	jz spin
	movb $0xe, %ah
	movw $0x7, %bx
	int $0x10
	jmp put_msg_loop

spin:
	jmp spin

Error_msg: .ascii "failed to read the sectors, exit"
			.byte 0
msgl:
		.ascii "Hello Night!!!\r\n"
		.ascii "\n"
		.byte 0

.org 510
end_signal :
	.word 0xaa55
