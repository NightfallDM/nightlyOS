// Mutual exclusion spin locks

void initlock(struct spinlock *lk, char *name){
	lk->name = name;
	lk->locked = 0;
	lk->cpu = 0;
};

int holding(struct spinlock *lk){
	int r;
	cli();
	// if locked and current cpu hold the lock,
	// return 0;
	r = lk->locked && lk->cpu==mycpu();
	sti();
	return r;
}

void acquire(struct spinlock *lk){
	cli(); // disable interrupt to avoid deadlock;
	if(holding(lk)){

		// Do not try to acquire the lock when the
		// current process hold the lock;
		panic("acquire");
	}
	
	// The xchg is atomic.
	while(xchg(&lk->locked, 1) != 0);

	// search "memory barrier" for more message
	// Or you can search the name of this func.
	__sync_synchronize(); // hardware, full barrier.

	lk->cpu = mycpu();
}

void release(struct spinlock *lk){
	if(!holding(lk)){
		panic("release");
	}

	lk->cpu = 0;
	
	// Make sure that the lock is cleared when
	// you ready to release the lock
	__sync_synchronize();

	asm volatile("movl $0, %0":: "+m" (lk->locked) :);
	sti();
}
