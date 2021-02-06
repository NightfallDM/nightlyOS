#ifndef _NTDM_BOOTPARAM_H
#define _NTDM_BOOTPARAM_H

#include"types.h"

struct setup_header{
	u32 kernel_size;
	u32 code32_start;		
}

struct e820_entry{
	u64 address;
	u64 size;
	u32 type;
}__attribute__((packed));

struct boot_params{
	u8 e820_entries;
	struct e820_entry* e820_table;
	struct setup_header* setup_hdr;
}__attribute__((packed));

#endif // end of "_NTDM_BOOTPARAM_H"
