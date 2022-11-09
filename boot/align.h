#ifndef HW_ALIGN_H
#define HW_ALIGN_H

static inline unsigned long palign(unsigned char palign, unsigned long num) {
	return (num & (1UL << palign)) ? (num + (1UL << palign)) & ~((1UL << palign) - 1) : num;
}

#endif /* HW_ALIGN_H */
