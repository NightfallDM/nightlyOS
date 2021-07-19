#include"bootparam.h"
#include"heap.h"
#include"types.h"
#include"mem_op.h"
/* use in asm */
struct boot_param __boot_param __attribute__((aligned(16)));

struct heap_hdr __hp_hdr;

/*
 * copy the "setup_header" to "__boot_param" in heap.
 * */
void copy_setup_header(void){
	u16 heap_addr = get_heap(&__hp_hdr, sizeof(struct setup_header));
	memcpy(heap_addr, &hdr, sizeof(hdr));
	__boot_param.setup_hdr = (struct setup_header *)heap_addr;
}

void init_heap(void){
	
}

void main(){

	// 10k place: 2k code, 8k heap
	heap_init(__hp_hdr, 0x8600, 8192);
	detect_memory();
	__boot_param.e820_entry = (struct e820_entry*)0x8600;
	// do not know how many entries at begin, so use then alloc.
	u16 _temp = get_heap(&__hp_hdr, 20*__boot_param.e820_entries);

	// copy "hdr" to "__boot_param"
	copy_setup_header();

	switch_protected_mode();
}
