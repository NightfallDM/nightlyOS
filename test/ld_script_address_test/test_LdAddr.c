#include<stdio.h>

void hello(){
	//printf("hello!\n");
}

int sum(int * array, int cnt){
	int sum = 0;
	for(int i=0;i<cnt;i++){
		sum += array[i];
	}
	return sum;
}

int main(){
	// put in ".bss" section by compiler
	static int noninit_array_int[10];
	
	//hello();

	// put in stack
	int init_array_int[5] = {1, 2, 3, 4, 5};
	int sum_init_array = sum(init_array_int, 5);
}
