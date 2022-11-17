#include "test/test.h"

static void mask_NMI(void){
	cli();
	outb(0x80, 0x70);
	io_delay();
}

static void enable_a20(void){
	asm volatile("pushw %%ax\n\t"
				"xorw %%ax, %%ax\n\t"
				"inb $0x92, %%al\n\t"
				"orb $0x2, %%al\n\t"
				"outb %%al, $0x92\n\t"
				"popw %%ax":);
}

#define A20_TEST_TIMES 32
#define A20_TEST_ADDR (4*0x80)
// pass - 1
static int a20_test(void) {
	int ok = 0;
	int ori, crt;
	int loops = A20_TEST_TIMES;
	u16 fs = get_fs();
	u16 gs = get_gs();

	set_fs(0x0);
	set_gs(0xffff);

	ori = crt = rdfs16(A20_TEST_ADDR);

	while(loops--) {
		wrfs16(++crt, A20_TEST_ADDR);
		io_delay();
		ok = rdgs16(A20_TEST_ADDR + 0x10) ^ crt;
		if (ok)
			break;
	}

	wrfs16(ori, A20_TEST_ADDR);
	set_fs(fs);
	set_gs(gs);
	return ok;
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
	if (!a20_test()) {
		puts("a20 failed!\n");
		while (1);
	}
	puts("a20 enable!\n");
	mask_all_interrupts();
	protected_switch();
}
