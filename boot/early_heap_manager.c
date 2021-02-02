#include"types.h"

/* use only in one segment in real mode*/

struct heap_hdr{
	u16 next_addr;
	u16 rest_size;
}__attribute__((packed))

/* the 4 byte before heap require free to put hdr*/
struct heap_hdr* init_heap(u16 start_addr, u16 size){
	struct heap_hdr* hp_hdr = (struct heap_hdr)start_addr;
	hp_hdr->next_addr = start_addr;
	hp_hdr->rest_size = size;
	return hp_hdr;
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
