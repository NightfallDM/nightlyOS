#include<stdio.h>

void main(void){
	int p = ((0x80100055+0xff) & ~(0x1000-1));
	printf("%X\n", p);
	int other = 0xfffff;
	printf("%X\n", other & ~(0x0));
	printf("%X\n", ~(0x0));
}
