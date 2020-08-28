//date: 2020/08/28

/*
 *kernel freelist relative
 * */

#include "types.h"
#include "string.h"

struct run {
	struct run *next;
};

struct {
	struct run *freelist;
}kmem;


//init memory between vstart and vend.
//Now just put every page in freelist,
//we will rebuild the pgdir after
void kinit1(void *vstart, void *vend){
	freerange(vstart, vend)
}


//free the first page mem after vstart to the vend,
//it means it align auto.
void freerange(void *vstart, void *vend){
	char* p; // convenient for Byte to Byte grow.
	p = (char *)PGROUNDUP(vstart);
	for(;p+PGSIZE<=(char *)vend;p+=PGSIZE){
		kfree(v);
	}
}


//free the page of physical memory pointed at by v.
void kfree(char *v){
	if((uint)v % PGSIZE || v<end || V2P(v)>PHYSTOP){
	//1.it means v should point at the addreess that align PGSIZE
	//We have do this by "PGROUNDUP"
	//2.the address should between "end" and "PHYSTOP" that
	//define in the "memlayout.h"

		//TO DO
		panic("kfree");
	}
	
	// fill with junk to catch dangling refs.
	memset_C(v, 1, PGSIZE); // "string.h"
	
	//put into freelist
	struct run *r;
	r = (struct run*)v;
	r->next = kmem.freelist;
	kmem.freelist = r;

}


//Allocate one 4096-byte page of physical mem.
//Return a pointer that the kernel can use.
//Return 0 if the memory cannot be allocated.

uint8_t * kalloc(void){
	struct run *r;
	
	r = kmem.freelist;
	if(r){
		//freelist just like a position pointer in list.
		//the last of this list do not init,So it.s freelist
		//is NuLL.(I think maybe call the freelist "freelist_node_p"
		// will be more suit.)
		kmem.freelist = r->next;
	}
}
