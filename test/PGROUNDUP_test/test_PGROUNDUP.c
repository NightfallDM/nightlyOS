#include "test_PGROUNDUP.h"
#include<stdio.h>

int main(void){
	char *p;
	p = (char*)PGROUNDUP((unsigned int)0x80103055);
	printf("%ld\n", (unsigned int)p);
}
