#include "trap.h"
#include "types.h"

// Local APIC registers, divided by 4 for use as uint32_t[] indices.

#define ID	(0x0020/4) // LAPIC ID Register
#define SVR	(0x0F0/4) // SPURIOUS Interrupt Vector Register
#define ENABLE	0x100 // software enable,set 8-bit in spurious register
#define TDCR	(0x3E0/4) // divide configuration register(for Timer)
#define TICR	(0x380/4) // initial Count Register(for Timer)
#define X1	0x0B // bits (0, 1, 3) set(1,1,1) mean divide by 1
#define TIMER	(0x320/4) // LVT Timer register
#define PERIODIC	(0x1<<17)
#define MASKED	0x10000 // Interrupt masked
#define LINT0	(0x0350/4)
#define LINT1	(0x0360/4)
#define VER	(0x030/4)
#define PCINT	(0x340/4)
#define ERROR	(0x370/4) // LVT Error Register
#define ESR	(0x280/4) // Error Status Register
#define EOI	(0x0b/4) // EOI register
#define BCAST	(0x1<<19) // bits 19-18 = 10 mean "all include self"
#define INIT	0x500 // INIT/RESET
#define ICRHI	(0x310/4) // Interrupt command register high
#define ICRLO	(0x300/4) // Interrupt command register low
#define TPR	(0x08/4)
#define LEVEL	0x8000 // Level triggered
#define DELIVS	(0x1<<12) // Delivery Status


volatile uint32_t *lapic; // initialized in "mp.c"


// lapic write: write val to specifical addr
static void lapicw(uint32_t index, uint32_t value){
	lapic[index] = value; // lapic registers are "mem map"
	lapic[ID]; // wait for write to finish.by reading
}

void lapicinit(void){
	if(!lapic)
		return;
	// 1.Enable local APIC (software Enable)
	// 2.Set spurious interrupt vector.
	lapicw(SVR, ENABLE|(T_IRQ0 + IRQ_SPURIOUS));

	// use "Periodic mode" APIC timer
	// 1. Make sure the "TPR" is set
	// 2. Set the local APIC timer's divide configuration register
	// 3. Configure the local APIC timer's interrupt vector and unmask the
	// timer's IRQ.
	// 4. Set the local APIC timer's initial count;
	// Get more detail in "https://wiki.osdev.org/APIC_timer"
	
	//TODO: set "TPR".
	//em... I do not find the figure fo "TRP" ...
	lapicw(TDCR, X1); // divide configuration register
	lapicw(TIMER, PERIODIC|(T_IRQ0 + IRQ_TIMER));
	lapicw(TICR, 10000000);

	// reference from "xv6-public"
	// Disable logical interrupt lines.
	lapicw(LINT0, MASKED);
	lapicw(LINT1, MASKED);
	
	// Disable performance counter overflow interrupts
	// on machine that provide that interrupt entry.
	if(((lapic[VER]>>16)& 0xFF) >=4)
		lapicw(PCINT, MASKED);

	// Map error interrupt to IRQ_ERROR.
	lapicw(ERROR, T_IRQ0 + IRQ_ERROR);

	// Clear error status register
	lapicw(ESR, 0);
	lapicw(ESR, 0);

	// Ack any outstanding interrupts.
	//
	// I/O apic would not send interrupt to this lapic unless
	// revice the ack.(I think...)
	lapicw(EOI, 0);

	// Send an init Level De-Assert to synchronise arbitration ID's
	// use Interrupt command register.
	//
	// Note: The Interrupt command register is made of two 32-bit registers
	// And the interrupt is issued when low register is written to(0x300),
	// So, you should write to high register first(0x310).
	lapicw(ICRHI, 0);
	lapicw(ICRLO, BCAST|INIT|LEVEL);
	

	// The "Delivery Status": 0 mean completed sending.
	// 			  1 mean not completed sending.
	// So, you should wait it complete.
	while(lapic[ICRLO] & DELIVS);

	// Enable interrupt on the APIC(but not on the processor).
	lapicw(TPR, 0);
}
