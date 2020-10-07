//author: night
//email: <1335715927@qq.com>
//date: 2020/08/25

/*
the first program after BIOS,stored at the 0 sector;
we set some env and switch from the real mode to the protected mode;
then we go jump to the "load.c" to read and load the kernel.
*/

#include "../include/asm/e820.h"
#include "asm.h"
.code16 //we are in real mode now :-)
.global start

start:
	xorw %ax, %ax
	movw %ax, %es
	movw %ax, %ds
	movw %ax, %ss

# get more sector data store beside this MAR
load_loader:
	movw $0x0202, %ax
	xorw %cx, %cx
	movw $0x80, %dx
	movw $0x7e00, %bx
	int $0x13
	jc panic_spin

# get_mm_info
# use e820h bios int to get the mem map;
# you can get more message in the below url
# http://www.ctyme.com/intr/rb-1741.htm
# reference from linux2.4/setup.S

#define SWAP 0x534d4150 // "SWAP"
# store the data at 0x0:0x400 (es:di)

get_mm_e820:
	//put the code just set once :-)
	movl $0x0, (E820NR)
	xorl %ebx, %ebx
	movw $E820MAP, %di
jmp_e820:
	xorl %eax, %eax
	movw $0xe820, %ax
	movl $20, %ecx
	movl $SWAP, %edx
	int $0x15
	jc bad_e820
	cmpl $SWAP, %eax
	jne bad_e820
	cmpl $E820_RAM, 16(%di)
//it means we just count the usable memory
//16(%di) means the "type" field in the mm_map address range descriptor
//see more detail in the URL gived before
	jne again_e820  # do not count,do not add the E820NR
good_e820:
	movl (E820NR), %eax
	cmpl $E820MAX, %eax
	jnl mm_map_finish
	incl (E820NR) # count
	movw %di, %ax
	add $20, %ax
	movw %ax, %di
again_e820:
	cmpl $0x0, %ebx
	jne jmp_e820

mm_map_finish:
	jmp set_A20

bad_e820:
	addl $100, (E820NR) # if err to read,then add 100,so you can check
	jmp mm88

mm88:
	movw $0x8800, %ax
	int $0x15
	movw %ax, (E820MAP+E820MAX*20) # after the E820MAP, xxx KByte(2-Byte)
	 


//Note that: now the A20 is clear,so the truth is you can acess all the address
//that in real mode.Because the address(0xffff:0xffff)need 21 lines.
//open the A20 so we can access more memory.

set_A20:
	inb $0x92, %al
	orb $0x2, %al
	outb %al, $0x92


//Now we are going to switch to the protected mode :-)
switch_protect:
	lgdt gdtr_value # contain the value should load into gdtr
//Note: do not use "$" before "gdtr_value" so it can load it's value 
	cli
	movl %cr0, %eax
	orl $0x1, %eax
	movl %eax, %cr0

//in the doc of intel,suggest a long jump follow the mode switch
//to refresh the segemention reg

//ljmp to the first seg(SEG_KCODE);
//secletor 1<<3 mean INDEX:0x1 TI:0x0 CPL:0x00
//see more detail about secletor in the intel doc
	ljmp $(1<<3), $start32


//Now we are in protected mode,start to use 32-bit code
.code32
start32:
	//set up the protected-mode data segment registers
	movw $0x1000, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movw $0x0, %ax
	movw %ax, %fs
	movw %ax, %gs

//set esp = $start,then call the c code "load.c"
//if return from the c code may mean we should panic :-(
	movl $start, %esp
	jmp (0x7e00) # the address behind this program

//panic and spin
panic_spin:
	jmp panic_spin


//set GDT
.p2align 2
gdt:
	SEG_NULLASM			       # null
	SEG_ASM(S_CODE|SC_RD, 0x0, 0xffffffff) # code seg base:0x0 limit:4G
	SEG_ASM(S_DATA|SD_WR, 0x0, 0xffffffff) # data seg base:0x0 limit:4G
//so,you can see that we just use the flat mode(only one segment)

gdtr_value:
	.word (gdtr_value - gdt -1)	#"-1" need because this ins bigger "1"
	.long gdt			#address gdt

.org 510
end:
	.word 0xaa55 
