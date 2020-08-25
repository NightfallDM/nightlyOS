//author: night
//email: <1335715927@qq.com>
//date: 2020/08/25

/*
the first program after BIOS,stored at the 0 sector;
we set some env and switch from the real mode to the protected mode;
then we go jump to the "load.c" to read and load the kernel.
*/

.code16 //we are in real mode now :-)
.global start

start:
	xorw %ax, %ax
	movw %ax, %es
	movw %ax, %ds
	movw %ax, %ss

//Note that: now the A20 is clear,so the truth is you can acess all the address
//that in real mode.Because the address(0xffff:0xffff)need 21 lines.
//open the A20 so we can access more memory.

set_A20:
	inb $0x92, %al
	xorb $0x2, %al
	outb %al, $0x92a


//Now we are going to switch to the protected mode :-)
switch_protect:
	lgdt gdtr_value //contain the value should load into gdtr
//Note: do not use "$" before "gdtr_value" so it can load it's value 
	movw %cr0, %ax
	xorb $0x1, %ax
	movw %ax, %cr0

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
	movw $start, %esp
	call loadmain

//panic and spin
panic_spin:
	jmp panic_spin


//set GDT
.p2align 2
gdt:
	SEG_NULLASM
	SEG_ASM(S_CODE|SC_RD, 0x0, 0xffffffff) // code seg base:0x0 limit:4G
	SEG_ASM(S_DATA|SD_WR, 0x0, 0xffffffff) // data seg base:0x0 limit:4G
//so,you can see that we just use the flat mode(only one segment)

gdtr_value:
	.word (gdtr_value - gdt -1)	//"-1" need because this ins bigger "1"
	.long gdt			//address gdt
