//reference from xv6
//Thanks!
//
//Memory layout

#define EXTMEM 0x100000  // start of extended memory
#define PHYSTOP 0xE000000 // Top physical memory
#define DEVSPACE 0xFE000000 // Other devices are at high address

#define KERNBASE 0x80000000
#define KERNLINK  (KERNBASE+EXTMEM)


#define V2P(a) (((uint32_t) (a)) - KERNBASE) 
#define P2V(a) ((void *) (((uint8_t *) (a)) + KERNBASE))

#define V2P_WO(x) ((x) - KERNBASE)
#define P2V_WO(x) ((x) + KERNBASE)
