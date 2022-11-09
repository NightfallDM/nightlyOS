#ifndef _NTDM_BOOT_H
#define _NTDM_BOOT_H

#include"types.h"
#include "stage2_data.h"
#include "detect_memory.h"

static inline cli(void){
	asm volatile("cli"::);
}

static inline io_delay(void){
	const u16 DELAY_PORT = 0x80;
	// "d": dx reg , "N" out ins "Matching constraint"
	asm volatile("outb %%al, %0"::"dN"(DELAY_PORT));	
}

/* put the byte in "AL" to port address in "DX"*/
static inline outb(u8 value, u16 port){
	asm volatile("outb %0, %1" ::"a"(value), "dN"(port));
}

static inline inb(u16 port){
	u8 value;
	asm volatile("inb %1, %0":"=a"(value):"dN"(port));
}

void load_kernel();
#endif // end of "_NTDM_BOOT_H"
