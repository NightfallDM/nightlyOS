#include "types.h"
#include "proc.h"

int nextpid = 1;
extern void forkret(void);
extern void trapret(void);
static proc *initproc;

struct {
	struct spinlock lock;
	struct proc proc[NPROC];
} ptable;

void pinit(void){
	initlock(&ptable.lock, "ptable");
}

// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and initialize state 
// required to run in the kernel.
// Otherwise return 0.
static struct proc*
allocproc(void){
	struct proc *p;
	char *sp;

	acquire(&ptable.lock);

	for(p=ptable.proc;p<ptable.proc+NPROC;p++){
		if(p->state == UNUSED){
			goto found;
		}
	}
	release(&ptable.lock);
	return 0;

found:
	p->state = EMBRYO;
	p->pid = nextpid++;
	release(&ptable.lock);

	// Allocate kernel stack.
	if((p->kstack = kalloc()) == 0){
		p->state = UNUSED;
		return 0;
	}
	
	// sp point to the top of the address that 
	// One page return by "kalloc".
	sp = p->stack + KSTACKSIZE;

	// Leave room for trap frame.
	sp -= sizeof *p->tf;
	p->tf = (struct trapframe*)sp;

	// Set up new context to start executing at forkret.
	// which return to trapret.
	sp -=4;
	*(uint32_t*)sp = (uint32_t)trapret;

	sp -= sizeof *p->context;
	p->context = (struct context*)sp;
	// clear the p->context
	mmset(p->context, 0, sizeof *p->context);

	p->context->eip = (uint32_t)forkret;

	return p;
}


// Set up first user process manually.
// Just call when build the first process
//
// Two point may should to focus on:
// 1. Put the content of the first process("initcode.S")
// 2. Let the process run at a correct address.(by set up 
// the appropriate stack and then use "ret" ins etc.)
void userinit(void){
	struct proc *p;
	// Set up by the linker (_binary_objname_[start|end|size])
	// because the name of "initcode.S", So the "objname"="initcode".
	// exactly the "initcode.o", but the linker seem do not use the suffix.
	extern char _binary_initcode_start[], _binary_initcode_size[];
	
	p = allocproc();

	initproc = p;

	// Set up user pdt
	if((p->pgdir = setupkvm()) == 0){
		panic("userinit: out of memory?");
	}

	inituvm(p->pgdir, _binary_initcode_start, _binary_initcode_size);
	p->sz = PGSIZE;
	
	// Set up the trapframe of process, to let it iret to the 
	// correct address.
	memset(p->tf, 0, sizeof *p->tf);
	p->tf->cs = (SEG_UCODE << 3)|DPL_USER; // SEG selector
	p->tf->ds = (SEG_UDATA <<3)|DPL_USER;
	p->tf-es = p->tf->ds;
	p->tf->ss = p->tf->ds;
	p->tf->eflags = FL_IF; // trap gate enable interrupt
	p->tf->esp = PGSIZE;
	p->tf->eip = 0; // beginning of initcode.S


	safestrcpy(p->name, "initcode", sizeof p->name);
	p->cwd = namei("/");

	  // this assignment to p->state lets other cores
  	// run this process. the acquire forces the above
  	// writes to be visible, and the lock is also needed
  	// because the assignment might not be atomic.
  	acquire(&ptable.lock);
  	p->state = RUNNABLE;
  	release(&ptable.lock);

}
