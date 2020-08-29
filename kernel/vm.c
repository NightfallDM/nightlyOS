//data: 2020/08/29

#include "types.h"
#include "x86.h"
#include "memlayout.h"
#include "string.h"
#include "defs.h"

extern char* data[]; // define the symbol in "kernel.ld"
pde_t *kpgdir;
void switchkvm(void);

static pte_t *
walkpgdir(pde_t *pgdir, void *va, int32_t alloc){
	pde_t *pde;
	pte_t *pte;

	// 31-22 bit in "va" mean index of pgdir
	pde = pgdir+((uint32_t)va >> 22);


}

static int32_t 
mappages(pde_t *pgdir, uint32_t *va, uint32_t size, uint32_t pa,uint32_t perm){
	uint32_t *first_pg, *last_pg;
	pte_t *pte;

	first_pg = (uint32_t *)PGROUNDDOWN(va);// get first map page
	// get last map page
	last_pg = (uint32_t *)PGROUNDDOWN((uint32_t)va + size -1);

	for(;;){
		if((pte=walkpgdir(pgdir, first_pg, 1)) == 0){
			return -1;
		}
		//TODO
	}
}

// see more detail in "xv6-public/vm.c"
static struct kmap{
	uint32_t * va;
	uint32_t * phy_start;
	uint32_t * phy_end;
	int32_t perm;
}kmap[] = {
 { (uint32_t *)KERNBASE, 0, EXTMEM, PTE_W}, // I/O space
 { (uint32_t *)KERNLINK, V2P(KERNLINK, V2P(data), 0)}, //kern test+rodata
 { (uint32_t *)data,     V2P(data), PHYSTOP, PTE_W}, //kern data+memory
 { (uint32_t *)DEVAPACE, DEVSPACE, 0, PTE_W}, // more devices
};

pde_t * setupkvm(void){
	pde_t * pgdir;
	struct kmap *k;

	// take one page that in the head of freelist as pgdir,
	// Return 0 mean freelist is empty.
	if((pgdir=(pde_t *)kalloc()) == 0){
		panic("freelist empty");
	}
	memset_C(pgdir, 0, PGSIZE);
	if (P2V(PHTSTOP) > (void *) DEVSPACE){
		panic("PHYSTOP too high");
	}
	for(k=kmap; k<(kmap+NELEM(kmap)); k++){
		if(mappages(pgdir, k->va, k->phy_end - k->phy_start, 
		   k->phy_start, k->perm) < 0){
			// TODO
			freevm(pgdir);
			return 0;
		}
	return pgdir;
	}
	
}


// Rebuild the kernel page table
// And switch to it.
void kvmalloc(void){
	kpgdir = setupkvm();
	switchkvm();
}

void switchkvm(void){
	lcr3(V2P(kpgdir));
}
