#ifndef STRING_H
#define STRING_H

// give the size to unsigned short because we use cx to carry size (-mregparm)
void memcopy_es(void* dest, void* src, unsigned short size, unsigned short es);
void memcopy(void* dest, void* src, unsigned short size);
#endif /* STRING_H*/
