#include"bootparam.h"
#include"heap.h"
#include"types.h"
/* use in asm */
struct boot_param __boot_param;

struct heap_hdr __hp_hdr;
void main(){

	// 10k place: 2k code, 8k heap
	heap_init(__hp_hdr, 0x8600, 8192);
	detect_memory();
	__boot_param.e820_entry = (struct e820_entry*)0x8600;
	// do not know how many entries at begin, so use then alloc.
	u16 _temp = get_heap(&__hp_hdr, 20*__boot_param.e820_entries);

	switch_protected_mode();
}
