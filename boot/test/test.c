#include "test.h"
#include "../string.h"

#define BUFFER_HEIGHT 25
#define BUFFER_WIDTH 80

int heap_end = (int)_end;
int heap_top = ((int)_end) + HEAP_SIZE;

typedef struct {
	u32 cusor_pos_x;
	u32 cusor_pos_y;
}cusor_pos_t;

// static volatile cusor_pos_t cusor;

static inline unsigned int strlen(const char *str) {
	unsigned int ret = 1;
	while (*(str++) != 0) {
		ret++;
	}
	return ret;
}


static void store_video(void) {
	unsigned short col, mode, page_num;
	asm volatile("movw $0xf00, %%ax\n\t"
				 "int $0x10"
				 :"=a"(col), "=b"(page_num)
				 :
				 :"memory");
	mode = col & 0xff;
	col >>= 8;
	volatile unsigned short *msg_buff = (volatile unsigned short*)get_heap(sizeof(unsigned short) * 3);
	msg_buff[0] = col;
	msg_buff[1] = mode;
	msg_buff[2] = page_num;
}

static cusor_pos_t store_cusor(void) {
	cusor_pos_t ret_cusor;
	unsigned cusor_start_line, cusor_end_line, pos_y, pos_x;
	asm volatile("movw $0x300, %%ax\n\t"
				 "xorw %%bx, %%bx\n\t"
				 "int $0x10"
				 :"=c"(cusor_start_line), "=d"(pos_y)
				 :
				 :"memory");
	cusor_end_line = cusor_start_line & 0xff;
	cusor_start_line >>= 8;
	pos_x = pos_y & 0xff;
	pos_y >>= 8;
	ret_cusor.cusor_pos_x = pos_x;
	ret_cusor.cusor_pos_y = pos_y;
	volatile unsigned short *msg_buff = (volatile unsigned short*)get_heap(sizeof(unsigned short) * 4);
	msg_buff[0] = cusor_start_line;
	msg_buff[1] = cusor_end_line;
	msg_buff[2] = pos_y;
	msg_buff[3] = pos_x;
	return ret_cusor;
}

void main(void) {
		cusor_pos_t cusor;
		// int a = 100 + 100;
		//char msg[] = "aaaaaaaaaaa\0";
		unsigned short fs_reg;
		//int _end_addr = (unsigned int)_end;
		//_end_addr += 0x10000;

		volatile char *heap_buff = (volatile char*)_end;
		for (int i = 0; i < 48; i++) {
			heap_buff[i] = i;
		}

		asm volatile("": : :"memory");
		store_video();
		asm volatile("": : :"memory");
		cusor = store_cusor();
		asm volatile("": : :"memory");
		fs_reg = get_fs();
		set_fs(0xb800);
		//volatile unsigned short *vga_buff = (volatile unsigned short*)(0x0 + (0xa - 1) * BUFFER_WIDTH * 2);
		volatile unsigned short *vga_buff = (volatile unsigned short*)((0x0 + (cusor.cusor_pos_y - 1) * BUFFER_WIDTH + cusor.cusor_pos_x) * 2);
		//volatile unsigned short *vga_buff = (volatile unsigned short*)0xa0;

		//int len = strlen(msg);
		for (int i = 0; i < BUFFER_WIDTH; i++) {
			//vga_buff[i] = (0xb << 8 ) | ' ';

			// vga_buff[i] = (0xb << 8 ) | 0x61;
			unsigned short val = (0xb << 8) | 0x61;
			wrfs16(val, (unsigned int)vga_buff + i * 2);
			//vga_buff[i] = 0;
		}
		set_fs(fs_reg);

		puts("!!!hello_night!!!!!\n");
		switch_protected_mode();
		while (1) {}
}
