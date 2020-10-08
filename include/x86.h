/*
 *Hold the platform dependence instruction
 * */

//reference from xv6
//Thanks!!!

#include "types.h"

#ifndef __X86_H_
#define __X86_H_

static inline void
stosb(void *addr, int data, int cnt){
	asm volatile (
			"cld; rep stosb"
			: "=D" (addr), "=c" (cnt)
			: "0" (addr), "1" (cnt), "a" (data)
			: "memory", "cc"
			);
}


static inline void
outb(uint16_t port, uint8_t data){
	asm volatile ("outb %0, %1": : "a" (data), "d" (port));
}


static inline uint8_t
inb(uint16_t port){
	uint8_t data;
	asm volatile ("inb %1, %0": "=a" (data) : "d" (port));
	return data;
}


static inline void
insl(uint32_t port, uint32_t dest_addr, uint32_t cnt){
	asm volatile (
			"cld; rep insl"
			: "=D" (dest_addr), "=c" (cnt)
			: "d" (port), "0" (dest_addr), "1" (cnt)
			: "memory" , "cc"
			);
}

static inline void
lcr3(uint32_t *addr){
	asm volatile (
			"movl %0, %%cr3"
			: 
			: "r" (addr)
			:
		     );
}

static inline void
cli(){
	asm volatile("cli":::);
}

static inline void
sti(){
	asm volatile("sti":::);
}

static inline int
xchg(int *addr, int newval){
	int ret;
	asm volatile("lock; xchgl %0, %1"
		     : "+m" (*addr), "=a" (ret)
		     : "1" (newval)
		     : "memory", "cc");
	return ret;
}

// Layout of the trap frame built on the stack by the hardware and by trapasm.S
// And passed to trap()
//
// Note that the order of "GPR" should obey the order of "pusha".
//
struct trapframe {
	// registers as pushed by pusha
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t oesp; // useless & ignore (original val of esp)
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;

	// rest of trap frame
	uint16_t gs;
	uint16_t padding1;
	uint16_t fs;
	uint16_t padding2;
	uint16_t es;
	uint16_t padding3;
	uint16_t ds;
	uint16_t padding4;
	uint32_t trapno;

	// below here defined by x86 hardware
	uint32_t err;
	uint32_t eip;
	uint16_t cs;
	uint16_t padding5;
	uint32_t eflags;

	// below here only when crossing ring, such as from user to kernel
	uint32_t esp;
	uint16_t ss;
	uint16_t padding6;
};

#endif //end __X86_H_
