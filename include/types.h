#ifndef _TYPES_H_
#define _TYPES_H_

#define MACHINE_WIDTH 32

typedef unsigned short uint16_t;
typedef short int16_t;
typedef char int8_t;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef int uint32_t;

struct Page_e {
	uint32_t attr: 12;
	uint32_t addr: 20;
}__attribute__((packed));

typedef struct Page_e pde_t;
typedef struct Page_e pte_t;

#endif //_TYPES_H_
