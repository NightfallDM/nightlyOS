/* stage2 data area 
 * this area is used to store data in stage2 boot setup.
 *
 * location: behind ".end" declare in setup.ld align(4).
 * */
#ifndef HW_STAGE2_DATA_H
#define HW_STAGE2_DATA_H

#include "align.h"
#include "mem_op.h"

#define STAGE2_DATA_SIZE	0x1000
static unsigned int start_real_addr = 0;
static unsigned int data_offset = 0;

enum stage2_data_type{
	HDRS,
};

struct data_entry {
	stage2_data_type dt;
	unsigned char size; // byte
};

struct stage2_data_hdr {
	unsigned int nums;
	unsigned int data_offset;
	struct data_entry data_entrys[32];
};


void stage2_init_data_area() {
	extern unsigned int _end;
	start_real_addr = (_end & 0xff) ? (_end + 0x100) & (~1UL - 2) : _end;
	memset((void*)start_real_addr, 0, STAGE2_DATA_SIZE);
	struct stage2_data_hdr *sdh = (struct stage2_data_hdr*)start_real_addr;
	sdh->nums = 0;
	sdh->data_offset = palign(2, start_real_addr + sizeof(struct stage2_data_hdr));
	data_offset = sdh->data_offset;
}

// check init state first than call this
static inline unsigned char s2da_data_full() {
	return (struct stage2_data_hdr*)start_real_addr->nums < STAGE2_DATA_SIZE ? 0: 1;
}

static inline unsigned int s2da_curr_data_offset() {
	struct stage2_data_hdr *hdr = (struct stage2_data_hdr*)start_real_addr;
	unsigned int curr_offset = hdr->data_offset;
	for(unsigned int idx = 0; idx < hdr->nums; idx++) {
		curr_offset += hdr->data_entrys[idx].size;
	}
	return curr_offset;
}

#define STAGE2_DATA_NONINITED	-1;
// ret the entry real addr, can use direct in stage2 setup
// 0 failed (full)
int s2da_add_entry(stage2_data_type dt, unsigned char ds) {
	if (!start_real_addr) {
		return STAGE2_DATA_NONINITED;
	}
	if (s2da_data_full()) {
		return 0;
	}

	struct stage2_data_hdr *hdr = (struct stage2_data_hdr*)start_real_addr;
	int ret_addr = s2da_curr_data_offset();
	unsigned int nums = hdr->nums;
	hdr->data_entrys[nums].dt = dt;
	hdr->data_entrys[nums].size = ds;
	hdr->nums++;
	return ret_addr;
}

// ret the real addr of the data type entry
// 0 dont have
unsigned int s2da_find_entry(stage2_data_type dt) {

}

#endif /* HW_STAGE2_DATA_H */
