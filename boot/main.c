#include"bootparam.h"

/* use in asm */
struct boot_param __boot_param;

void main(){
	detect_memory();
	__boot_param.e820_entry = (struct e820_entry*)0x8600;
	switch_protected_mode();
}
