//date: 2020/08/28


/*
 *common string operations
 * */

#include "types.h"
#include "x86.h"


void* memset_C(void * addr, uint8_t c, uint32_t n){
	uint32_t startaddr = (uint32_t)addr;
	uint32_t not_align_cnt = (uint32_t)addr % 4;
	uint32_t res_cnt = n - not_align_cnt;
	for(;not_align_cnt>0;not_align_cnt--){
		*(uint8_t *)addr = c | 0xff;
		addr++;
	}
	for(;res_cnt-4>0;res_cnt-=4){
		c &= 0xff;
		*(uint32_t *)addr = (c<<24)|(c<<16)(c<<8)|c;
		addr++;
	}
	if(res_cnt != 0){
		for(;res_cnt>0;res_cnt--){
			*(uint8_t *)addr = c|0xff;
		}
	}
	return (void *)startaddr;
}
