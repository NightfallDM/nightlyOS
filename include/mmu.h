#ifndef __MMU_H_
#define __MMU_H_

// various segment selectors.
#define SEG_KCODE 	1 // kernel code
#define SEG_KDATA	2 // kernel data+stack
#define SEG_UCODE	3 // user code
#define SEG_UDATA	4 // user data+stack
#define SEG_TSS		5 // this process's task state

#define DPL_USER	0x3 // User DPL

// Application segment type bits
#define STA_X	0x8 // Exectable segment
#define STA_W	0x2 // Writeable (non-executable segment)
#define STA_R	0x2 // Readable (executable segments)

// Normal segment
#define SEG(type, base, lim, dpl) (struct segdesc)    \
{ ((lim) >> 12) & 0xffff, (uint)(base) & 0xffff,      \
  ((uint)(base) >> 16) & 0xff, type, 1, dpl, 1,       \
  (uint)(lim) >> 28, 0, 0, 1, 1, (uint)(base) >> 24 }


// see more detail in "test/PGROUNDUP_test/result*"
#define PGROUNDUP(sz) ((sz)+PAGESIZE-1) & ~(PAGESIZE-1)
//the first page address after address of "sz"

#define PGROUNDDOWN(sz) ((sz) & ~(PAGESIZE-1))
//the first page address before address of "sz"

#endif //end __MMU_H_
