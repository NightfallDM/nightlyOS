//author: night
//date: 2020/08/25

/*
 *The hdr file define for the BIOS int15 e820h
 * */

#ifndef __E820_H_
#define __E820_H_

#define E820MAP 0x404  //The address start to hold the mm_map list
#define E820MAX 32     //number of entries in E820MAP
#define E820NR 0x400   //use 4-Byte to hold the count of entries

#define E820_RAM 1     //available to OS
#define E820_RESERVED 2//not available(e.g. system ROM. memory-mapped device)
#define E820_ACPI 3    //usable as RAM once ACPI tables have been read
#define E820_NVS 4

#endif //end of __E820_H_
