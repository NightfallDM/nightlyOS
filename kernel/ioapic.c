#include "types.h"
#include "trap.h"

#define IOAPIC	0xFEC00000 // Default physical address of IO APIC
#define REG_ID	0x00 // Register index: ID
#define REG_VER	0x01 // Register index: version
#define REG_TABLE	0x10 // redirection table base


// The redirection table starts at REG_TABLE and uses
// two registers to configure each interrupt.
// The first(low) register in a pair contains configuration bits.
// The second(high) registers contains a bitmask telling which 
// CPUs can serve that interrupt.
// get more detail in : https://wiki.osdev.org/APIC

struct ioapic {
	uint32_t reg;

	// do not use.
	uint32_t pad[3]; // padding to make the read/write register align 16
	uint32_t data;
}

volatile struct ioapic *ioapic;

static uint32_t ioapicread(int reg){
	ioapic->reg = reg;
	return ioapic->data;
}

static void ioapicwrite(int reg, uint32_t data){
	ioapic->reg = reg;
	ioapic->data = data;
}


void ioapicinit(void){
	int i, id, maxintr;
	ioapic = (struct ioapic*)IOAPIC;
	maxintr = (ioapicread(REG_VER) >> 16) & 0xFF;
	id = ioapicread(REG_ID) >> 24;
	if(id !=ioapicid){
		//TODO
		cprintf("ioapicinit: id isn't equal to ioapicid; not a MP\n");
	}

	// Mark all interrupt edge-triggered, active high, disable;
	// do not route to any CPUs.
	for(i=0;i<maxintr;i++){
		ioapicwrite(REG_TABLE+2*i, INT_DISABLE|(T_IRQ0+i));
		ioapicwrite(REG_TABLE+2*i+1, 0);
	}
}

void ioapicenable(int irq, int cpunum){

	// Mark interrupt edge-triggered, active high,
	// enable, adn routed to the given cpunum,
	// which happens to be that cpu's APIC ID.
	ioapicwrite(REG_TABLE+2*irq, T_IRQ0 + irq);
	ioapicwrite(REG_TABLE+2*irq+1, cpunum << 24);
}
