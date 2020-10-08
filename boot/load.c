//author: night
//email: <1335715927@qq.com>
//date: 2020/08/26

/*
 *The boot1.S jump to this file;
 *we are going to load the ELF kernel image from the disk 
 *start at the sector 4;
 *we will load 4KByte from kernel to the physical address at 0x10000
 *to read the ELF header;
 *Then follow the header to load the whole kernel :-)
 * */

#include "../include/elf.h"
#include "../include/x86.h"
#include "../include/types.h"
void loadmain(void);

static void read_load(uint32_t *dest_addr, uint32_t count, uint32_t k_off);

static void _read_one_sector(uint32_t *dest_addr, uint32_t k_off);

static void _waitdisk(void);

void loadmain(void){
	struct elfhdr *elf; //elf header
	struct proghdr *ph, *eph;
	void (*entry)(void); // func point
	uint8_t *pa; //physical address

	elf = (struct elfhdr*)0x10000; //let elf point to this place in mm
	read_load((uint32_t*)elf, 4096, 0);

	//read the elf header and load program from disk to mm 
	while (elf->magic != ELF_MAGIC); 
	//because we jump to this prog from "boot1.S",but not call
	//So we can back :-) do not hand err,Just spin.
	
	ph = (struct proghdr*)((uint8_t)elf+elf->phoff);
	//get the address of program header
	
	eph = ph + elf->phnum; //eph means addr of the end proghdr
// because the ph is a pointer,So the add just means pointer add.
	
	//loop to load each program into memory
	for(; ph<eph; ph++){
		pa = (uint8_t*)ph->paddr;//means put this section in mm at paddr
		read_load(pa, ph->filesz, ph->off);
		if(ph->memsz > ph->filesz){
			//bss section, fill zero
			stosb(pa + ph->filesz, 0 ,ph->memsz - ph->filesz);
		}
	}

	//Call the entry point from the ELF hdr by the elf->entry
	//Does not return!
	
	entry = (void (*)(void))(elf->entry); //let the func pointer point to
// the elf->entry,Then we can call it
	entry();
}

// read count Byte data from the kernel->k_off,then put them at dest_addr
static void read_load(uint32_t *dest_addr, uint32_t count, uint32_t k_off){
	int start = 0;
	for (int i=0;i<(int)(count/512)+1;i++){
		_read_one_sector(dest_addr+512*i, k_off+512*i);
	}
}

// read one sector from the kernel->koff,then put them at desct_addr
static void _read_one_sector(uint32_t *dest_addr, uint32_t k_off){
	/*
	 *because we now in the protected mode, so we can not call
	 *the BIOS int,So we use LBA-28 to read data from the disk
	 *
	 * Note: if your disk more than 128 G,you may should to use
	 * LBA-48, and change it :-)
	 * */
	_waitdisk();
	// LBA-28
	outb(0x1F1, 0);
	outb(0x1F2, 1);
	outb(0x1F3, k_off);
	outb(0x1F4, k_off >> 8);
	outb(0x1F5, k_off >> 16);
	outb(0x1F6, (((k_off << 4) >> 24)|0xE0)); 
	// Note that the code in xv6 may have bug,So i have changed it :-)
	outb(0x1F7, 0x20);

	//Read data.
	_waitdisk();
	insl(0x1F0, dest_addr, 512);

}

// wait for disk ready
static void _waitdisk(void){
	while((inb(0x1F7) & 0xC0) != 0x40); // if do not ready just loop
}
