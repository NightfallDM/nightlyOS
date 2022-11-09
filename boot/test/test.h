#ifndef TEST_H
#define TEST_H

#include "test_types.h"

#define HEAP_SIZE 0x2000
#define STACK_SIZE 0x2000

typedef char bool;
#define true 1
#define false 0

extern char _end[];
//static int heap_end = (int)_end;
//static int heap_top = ((int)_end) + HEAP_SIZE;
extern int heap_end;
extern int heap_top;

static inline unsigned short get_ds(void) {
	unsigned short seg;
	asm("movw %%ds,%0" : "=rm"(seg));
	return seg;
}

static inline unsigned short get_fs(void) {
	unsigned short seg;
	asm("movw %%fs,%0" : "=rm"(seg));
	return seg;
}

static void inline set_ds(unsigned short val) {
	asm volatile("movw %0, %%ds" : : "rm"(val));
}

static inline void set_fs(unsigned short val) {
	asm volatile("movw %0, %%fs" : : "rm"(val));
}

static inline void wrfs16(u16 val, u32 addr) {
	u16 *ptr = (u16*)addr;
	asm("movw %1, %%fs:%0": "+m" (*ptr) : "qi" (val));
}

static inline int heap_free(unsigned int size) {
	return heap_top - heap_end >= size? 1: 0;
}

static int get_heap(int size) {
	int ret = heap_end;
	if (heap_free(size)) {
		heap_end += size;
	}
	return ret;
}

static inline void io_delay(void) {
	const unsigned short DELAY_PORT = 0x80;
	asm volatile("outb %%al,%0" : : "dN" (DELAY_PORT));
}

/* tty.c start */
void puts(const char*);
void putchar(const char);
/* tty.c end */

#endif /* end of TEST_H */
