#ifndef _GDT_H_
#define _GDT_H_

/*
 *date: 2020/08/19
 *author: night
 * */

//this hdr file use to hold the structure about gdt and will use in the 
//"gdt.c"

#include"types.h"

struct idt_entry{
	uint16_t seg_limit_0_15;
	uint16_t base_addr_0_15;
	uint8_t base_addr_16_23;
	uint8_t access;
	unsigned seg_limit_16_19: 4;
	unsigned flags : 4;
	uint8_t base_addr_24_31;	
}__attribute__((packed));
typedef idt_entry IDT_entry;



#endif //end of _GDT_H_
