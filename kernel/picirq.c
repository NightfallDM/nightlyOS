#include "types.h"
#include "x86.h"
#include "traps.h"

// I/O Address of the two Programmable interrupt controllers
#define IO_PIC1	0x20 // Master (IRQs 0-7)
#define IO_PIC2	0xA0 // Slave (IRQs 8-15)

// Disable the 8259A interrupt controllers, Because we use APIC for SMP.
void picinit(void){
	// mask all interrupts
	outb(IO_PIC1+1, 0xFF);
	outb(IO_PIC2+1, 0xFF);
}
