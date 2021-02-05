#include"boot.h"

static void mask_NMI(void){
	cli();
	outb(0x80, 0x70);
	io_delay();
}

static void enable_a20(void){
	asm volatile("inb $0x92, %%al\n\t"
			"orb $0x2, %%al\n\t"
			"outb %%al, $0x92");
}

/* ctl PIC to mask intrrupt*/
static void mask_all_interrupts(void){
	outb(0xff, 0xa1);
	io_delay();
	outb(0xfb, 0x21);
	io_delay();
}

void switch_protected_mode(void){
	mask_NMI();
	enable_a20();
	mask_all_interrupts();
	protected_switch();

}
