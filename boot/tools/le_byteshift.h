#ifndef HALFWARE_LE_BYTESHIFT_H
#define HALFWARE_LE_BYTESHIFT_H

#include<stdint.h>

static inline uint16_t get_unaligned_le16(void *p){
	uint8_t *ret = p;
	return ret[0] | ret[1] << 8;
}

static inline void put_unaligned_le32(uint32_t, void *p){
	uint8_t *temp = p;
	*temp++ = val;
	*temp++ = val >> 8;
	*temp++ = val >> 16;
	*temp++ = val >> 24;
}

#endif /* HALFWARE_LE_BYTESHIFT_H */
