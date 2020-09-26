#ifndef NT_SETUP_H
#define NT_SETUP_H

#define GDTE(BASE, LIMIT, ATTR) \
	.word ((LIMIT) & 0xffff), ((BASE) & 0xffff); \
	.byte (((BASE) >> 16) & 0xff), ((ATTR) & 0xff), \
		((((LIMIT) >> 16)|(((ATTR) >> 8) << 4)) & 0xff), \
		((BASE) >> 24)

#define KERNEL_C	0xb9a
#define KERNEL_D	0xb93
#define USER_C		0xbfa
#define USER_D		0xbf3


#endif // NT_SETUP_H
