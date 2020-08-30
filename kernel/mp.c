//date: 2020/08/30

/*
 *MultiProcessor support
 *Search memory for MP description structures.
 * */

#include "types.h"
#include "mp.h"
#include "proc.h"

struct cpu cpus[NCPU];
uint32_t ncpu;
uint8_t ioapicid;



uint32_t sum(uint8_t *addr, uint32_t size){
	uint32_t i, sum;
	sum = 0;
	for(i=0;i<size;i++){
		// "addr" set up as "uint8_t",
		// So it setp 1 Byte once.
		sum+=addr[i];
	}
	return sum;
}

// Look for an MP structure in the len bytes at addr
static struct mp*
search1(uint32_t addr, uint32_t len){
	uint8_t *end_addr, *start_addr, *p;
	start_addr = P2V(addr); // P2V cast the type to uint32_t
	end_addr = start_addr + len; // So the add mean (uint32_t) + (uint32_t)
	
	p = start_addr;
	for(; p<end_addr;p+=sizeof(struct mp)){
		if(memcmp(p, "_MP_", 4) == 0 && sum(p, sizeof(struct mp)) == 0)
		{
			return (struct mp*)p;
		}
		return 0
	}
}


// Search for the MP Floating Pointer Structure,Which according to the
// spec is in one of the following three locations:
// 1) in the first KB of the EBDA
// 2) in the last KB of system base memory;
// 3) in the BIOS ROM between 0xE0000 adn 0xFFFFF.
static struct mp*
mpsearch(void){
	// set bde as "uint8_t" for step to step with 1 Byte;
	uint8_t * bde = (uint8_t *)P2V(0x400);

	struct mp *mp;
	uint32_t p; // start search addr;

	// Note that "(bad[0x0F] << 8)|bda[0x0E]" because the 
	// little-endian,careful the order of byte :-)
	if(((p=bda[0x0F]<<8)|bda[0x0E]) << 4){
		if((mp=search1(p, 1024))){
			return mp;
		}
	}else{
		// Note : same whit the reason before;
		// And there is length of system base memory with KByte
		// So, "*1024" needed.
		p = ((bda[0x14]<<8)|bda[0x13])*1024;
		if((mp=search1(p-1024, 1024))){
			return mp;
		}
	}
	return mpsearch1(0xF0000, 0x10000); // 0xF0000~0xFFFFF
}


static struct mpconf*
mpconfig(struct mp **pmp){
	struct mpconf *conf;
	struct mp *mp;

	// "mpsearch" search the MP Floating Pointer Structure
	if((mp=mpsearch()) == 0 || mp->physaddr == 0){
		return 0;
	}

	conf =(struct mpconf*)P2V(mp->physaddr);
	// do some check for the conf structure
	
	if(memcmp(conf, "PCMP", 4) != 0){
		return 0;
	}
	if(conf->version !=1 && conf->version !=4){
		return 0;
	}

	// cast the "conf" as "uint8_t *" for Byte step.
	if(sum((uint8_t *)conf, conf->length) != 0){
		return 0;
	}

	*pmp = mp;
	return conf;
}

void mpinit(void){
	struct mp *mp;
	struct conf *conf;
	struct mpproc *proc;
	struct mpioapic *ioapic;
	bool_t ismp;
	uint8_t *p; // Byte steper
	extern lapic; // define in the "lapic.c"

	// You see, we do not init the mp, but we still pass the 
	// "&mp", because we want to let the "mpconfig" return two 
	// value, this is the trick in C  :-)
	if((conf=mpconfig(&mp)) == 0){
		panic("Expect to run on an SMP");
	}
	ismp = TRUE;
	lapic = (uint32_t *)conf->lapicaddr; // !cast the result, not "conf"

	
	for(p=(uint8_t *)(conf+1), e=(uint8_t *)conf + conf->length;p<e;){
		// the type of p is "uint8_t",
		// So, *p mean the first Byte value 
		// The first 1-Byte is "type" of each entry
		switch(*p){
			case MPPROC:
				proc = (struct mpproc*)p;
				
				//TODO
				if(ncpu < NCPU){
					// apicid may different from ncpu
					cpus[ncpu].apicid = proc->apicid;
					ncpu++;
				}
				p += sizeof(struct mpproc);
				continue;
			case MPIOAPIC:
				ioapic = (struct mpioapic*)p;
				ioapicid = ioapic->ioapicno;
				p += sizeof(struct mpioapic);
				continue;
			
			case MPBUS:
			case MPIOINTR:
			case MPLINTR:
				//8 Byte length
				p+=8;
				continue;
			default:
				ismp = FALSE;
				break;
		}
	}

	if(!ismp){
		panic("Didn't find a suitable machine");
	}
	
	// TODO
	// NOT KNOW
	//
	// IMCR
	if(mp->imcrp){
		outb(0x22, 0x70); // Select IMCR
		outb(0x23, inb(0x23) | 1); // Mask external interrupt.
	}
}
