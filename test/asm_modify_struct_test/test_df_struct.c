#include<stdio.h>

extern void change(void);

typedef struct Test{
	int num1;
	int num2;
	unsigned int num3;
}test;

void main(){
	test test1 = {111, 222, 333};
	test* ptest1 = &test1;
	change();
	printf("num1 = %d\nnum2 = %d\nnum3 = %d\n", test1.num1, test1.num2, test1.num3);
	printf("test1 cost %dbytes\n", sizeof(test));
	int num4 = 444;
	change();
	printf("num4 = %d\n", num4);
}
