/*
 *the hdr file hold the struct about the ELF format
 * */

//reference from xv6/elf.h
#include "types.h"

#ifndef __ELF_H_
#define __ELF_H_

#define ELF_MAGIC 0x464c457FU // elf_magic_num

// ELF header
struct elfhdr {
	uint32_t magic;
	uint8_t elf[12];
	uint16_t type;
	uint16_t machine;
	uint32_t version;
	uint32_t entry;
	uint32_t phoff;
	uint32_t shoff;
	uint32_t flags;
	uint16_t ehsize;
	uint16_t phentsize;
	uint16_t phnum;
	uint16_t shentsize;
	uint16_t shnum;
	uint16_t shtrendx;
}__attribute__((packed));

// Program header
struct proghdr {
	uint32_t type;
	uint32_t off;
	uint32_t vaddr;
	uint32_t paddr;
	uint32_t filesz;
	uint32_t memsz;
	uint32_t flags;
	uint32_t ailgn;
}__attribute__((packed));


#endif //end __ELF_H_
