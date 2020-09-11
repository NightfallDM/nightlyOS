#include "types.h"
#include "proc.h"

struct {
	struct spinlock lock;
	struct proc proc[NPROC];
} ptable;

void pinit(void){
	initlock(&ptable.lock, "ptable");
}

void userinit(void){

}
