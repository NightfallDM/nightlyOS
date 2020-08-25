//author: night
//email: <1335715927@qq.com>
//date: 2020/08/25

/*
 *The boot1.S call this file and do not return if all pass;
 *we are going to load the ELF kernel image from the disk 
 *starting at sector 1 and jump to the kernel entry routine.
 * */

#include "elf.h"
#include "x86.h"

