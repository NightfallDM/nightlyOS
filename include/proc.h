//Per-process state

struct cpu {
	uint8_t apicid;		// Local APIC ID
	struct context *scheduler;	// swtch() here to enter scheduler
	struct taskstate ts;	// Used by x86 to find stack for interrupt
	struct segdesc gdt[NSEGS];	// x86 global description table
	volatile uint32_t started;	// Has the CPU started?
	//int ncli;	// Depth of pushcli nesting.
	struct proc *proc;	// The process running on this cpu or null
};

extern struct cpu cpus[NCPU];
extern int ncpu;

// TODO: maybe "xv6" use "context" and "trapframe" together to 
// represent the state.
struct context {
	uint32_t edi;
	uint32_t esi;
	uint32_t ebx;
	uint32_t ebp;
	uint32_t eip;
}

struct proc {
	uint32_t sz;	// size of process memory(bytes)
	pde_t *pgdir;	// Page tables
	char *kstack;	// Bottom of kernel stack for this process
	enum procstate state;	// Process state
	int pid;	// Process ID
	struct proc *parent;	// Parent process
	struct trapframe *tf;	// Trap frame for current syscall
	struct context *context;	//swtch() here to run process
	void *chan;	// If non-zero,sleeping on chan(TODO:do not know)
	int killed;	// If non-zero, have been killed
	struct file *ofiles[NOFILE];	// Open files
	struct inode *cwd;	// Current directory
	char name[16];	// Process name
};
