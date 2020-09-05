//data: 2020/08/29

#include "types.h"
#include "x86.h"
#include "memlayout.h"
#include "string.h"
#include "defs.h"
#include "proc.h" // "cpu" struct
#include "mmu.h" // "segment"

extern char* data[]; // define the symbol in "kernel.ld"
pde_t *kpgdir;
void switchkvm(void);

// Set up CPU's kernel segment descriptors.
// Run once on entry on each CPU.
void seginit(void){
	struct cpu *c;
	c = &cpus[cpuid()]; // current cpu
	// All set to 4 Gbyte segment
	c->gdt[SEG_KCODE] = SEG(STA_X|STA_R, 0, 0xffffffff, 0);
	c->gdt[SEG_KDATA] = SEG(STA_W, 0, 0xffffffff, 0);
	c->gdt[SEG_UCODE] = SEG(STA_X|STA_R, 0, 0xffffffff, DPL_USER);
	c->gdt[SEG_UDATA] = SEG(STA_W, 0, 0xffffffff, DPL_USER);
	lgdt(c->gdt, sizeof(c->gdt));
}

static pte_t *
walkpgdir(pde_t *pgdir, void *va, int32_t alloc){
	pde_t *pde;
	pte_t *pte;

	// 31-22 bit in "va" mean index of pgdir
	pde = pgdir+((uint32_t)va >> 22);
	
	// the entry in pgdir have set PTE_P mean the pgtable have 
	// already alloc,So do not need to alloc, just return the 
	// address
	if(*pde|PTE_P){

		// Note: !!! The high 20-bit of *pde hold the 
		// physical address of the pte
		// So ...
		pte = V2P((uint32_t)(*pde) & ~(0xfff))
	}else{
		if(!alloc || (pte = (pte *)kalloc()) == 0){
			return 0;
		}

		memset_C(pte, 0, PGSIZE);

		// Note: !!! The high 20-bit of *pde hold the 
		// physical address of the pte
		// So ...
		*pde = P2V(pte)|PTE_P|PTE_W|PTE_U;
	}

	return ((pte_t *)pte+(((uint32_t)va >> 12) & 0x3ff));


}

static int32_t 
mappages(pde_t *pgdir, uint32_t *va, uint32_t size, uint32_t pa,uint32_t perm){
	uint32_t *first_pg, *last_pg;
	pte_t *pte;

	first_pg = (uint32_t *)PGROUNDDOWN(va);// get first map page
	// get last map page
	last_pg = (uint32_t *)PGROUNDDOWN((uint32_t)va + size -1);

	for(;;){
		// pte point at the index of the entry in pgtab;
		if((pte=walkpgdir(pgdir, first_pg, 1)) == 0){
			return -1;
		}

		if(*pte|PTE_P){
			panic("remap");
		}

		// make pa align 4-KByte
		*pte = (pa & ~(0xfff))|perm|PTE_P;
		if(first_pg == last_pg){
			break;
		}
		first_pg += PGSIZE;
		pa += PGSIZE;
	}
	return 0;
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
