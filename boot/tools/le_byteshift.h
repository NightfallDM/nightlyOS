#ifndef HALFWARE_LE_BYTESHIFT_H
#define HALFWARE_LE_BYTESHIFT_H

#include<stdint.h>

static inline uint16_t get_unaligned_le16(void *p){
	uint8_t *ret = p;
	return ret[0] | ret[1] << 8;
}

#endif /* HALFWARE_LE_BYTESHIFT_H */
