#include<stdio.h>
extern int add(int, int);

int main(){
	int a = 1;
	int b = 2;
	int c = add(a, b);
	printf("c = %d\n", c);
}
