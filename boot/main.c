/* jump from the header.S */

#include"bootparam.h"
#include"heap.h"
#include"types.h"
#include"mem_op.h"
#include "stage2_data.h"
#include "boot.h"

/* use in asm */
// struct boot_param __boot_param __attribute__((aligned(16)));

struct heap_hdr __hp_hdr;

/*
 * copy the "setup_header" to "__boot_param" in heap.
 * */
static inline void copy_setup_header(void){
	struct setup_header *setup_hdr = (struct setup_header *)0x11f2;
	struct setup_header *hdr_entry = s2da_add_entry(HDRS, sizeof(struct setup_header));
	memcpy((void*)hdr_entry, setup_hdr, sizeof(struct setup_header));
}

void init_heap(void){
	
}

void main(){

	stage2_init_data_area();

	detect_memory();
	__boot_param.e820_entry = (struct e820_entry*)0x8600;
	// do not know how many entries at begin, so use then alloc.
	u16 _temp = get_heap(&__hp_hdr, 20*__boot_param.e820_entries);

	// copy "hdr" to "__boot_param"
	copy_setup_header();

	load_kernel();

	switch_protected_mode();
}
