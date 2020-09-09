//Per-process state

struct proc {
	uint32_t sz;	// size of process memory(bytes)
	pde_t *pgdir;	// Page tables
	char *kstack;	// Bottom of kernel stack for this process
	enum procstate state;	// Process state
	int pid;	// Process ID
	struct proc *parent;	// Parent process
	struct trapframe *tf;	// Trap frame for current syscall
	struct
}
