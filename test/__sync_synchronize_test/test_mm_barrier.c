int main(void){
	int x, y;
	x=4;
	__sync_synchronize();
	y=3;
	return 0;
}
