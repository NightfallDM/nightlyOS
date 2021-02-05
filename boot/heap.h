#ifndef _NTDM_HEAP_H
#define _NTDM_HEAP_H

#include"types.h"

struct heap_hdr{
	u16 next_addr;
	u16 rest_size;
}

#endif // end of "_NTDM_HEAP_H"
