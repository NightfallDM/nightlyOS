#include"types.h"
#include"heap.h"
/* use only in one segment in real mode*/

void init_heap(struct heap_hdr hp_hdr, u16 start_addr, u16 size){
	hp_hdr->next_addr = start_addr;
	hp_hdr->rest_size = size;
}

/* return the address*/
u16 get_heap(struct heap_hdr* hp_hdr, u16 size){
	if(size > hp_hdr->rest_size){
		return 0; // fail to alloc
	}
	u16 addr = hp_hdr->next_addr;
	hp_hdr->next_addr += size;
	hp_hdr->rest_size -= size;
	return addr;
}
