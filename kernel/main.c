/*
 *entry jump to main and going to execute :-)
 * */

//"end" symbol export in the "kernel.ld" use "PRIVODE",and let the
//"end" be the first Byte behind kernel.
extern end[];


int main(void){
	kinit1(end, P2V(4*1024*1024)); // in "kalloc.c"
	// phy page allocator, because Now we just have two pde in our
	// pgdir(pde[0] and pde[512]), the two pde refer to the same phyaddr
	// 0:0x400000
	// So this func means init the address behind kernel and below
	// 4 MByte,and put them in the freelist, we will use them to 
	// rebuild kpgdir in the "kvmalloc" func.
	
	kvmalloc(); // in "vm.c"
	// kernel page table.
	// Rebuild the page table use the detail address message,
	// More complete than the "pgdirentry",Because we do not konw
	// how many memory that use to load the kernel,and what the detail
	// address every section in kernel,but we put some point in kernel
	// by use "kernel.ld" like "data" and "end".
	// So now we have more message about the kernel.
	
	mpinit(); // in "mp.c"
	// detect other processors by the "MP Floating Pointer Structure"
	// see more detail in "Inter MultiProcessor specification 1.4"
	
	lapicinit(); // in "lapic.c"
	// interrupy controller
	
	seginit(); // in "vm.c"
	// segment descriptors
}
