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


static inline void
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

#endif //end __X86_H_
