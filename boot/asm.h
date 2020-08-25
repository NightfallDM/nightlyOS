//author: night
//email: <1335715927@qq.com>
//date: 2020/08/25

/*
 *put some marco that will use in the ".S" file
 * */

//set GDT_entry for initialization
//see more detail in xv6-public/asm.h  Thanks to them :-)
//read the doc provide by the intel and get more message

// the first descriptor in GDT do not use
#define SEG_NULLASM		\
	.word 0, 0;		\
	.byte 0, 0, 0, 0

//we set the "G" flag, so the every seg max size is "4G"
#define SEG_ASM(type,base,lim)				\
	.word (((lim)>>12)&0xffff), ((base)&0xffff);	\
	.byte (((base)>>16)&0xff), ((type)|0x90),	\
	      ((((lim)>>28)&0xf)|0xc0), (((base)>>24)&0xff)		

//Note: 0xc0 mean we set the "G" flag and the "D/B" flag,
//and the shift of lim should match,first rshift 12 mean the seg
//in 4-KByte increments



//we do not use the segement,so just simple two 4G size segement at the 
//same address (start at 0x0);
//so just define some simple marco below.
#define S_CODE 0x8
#define S_DATA 0x0
#define SC_RD 0x2
#define SD_WR 0x2
