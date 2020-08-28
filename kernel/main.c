/*
 *entry jump to main and going to execute :-)
 * */

//"end" symbol export in the "kernel.ld" use "PRIVODE",and let the
//"end" be the first Byte behind kernel.
extern end[];


int main(void){
	kinit1(end, P2V(4*1024*1024));
	// phy page allocator, because Now we just have two pde in our
	// pgdir(pde[0] and pde[512]), the two pde refer to the same phyaddr
	// 0:0x400000
	// So this func means init the address behind kernel and below
	// 4 MByte,and put them in the freelist, we will use them to 
	// rebuild kpgdir in the "kvmalloc" func.
	
}
