// Console input and output.
// Input is from the keyboard or serial port.
// Output is written to the screen and serial port.
//
// I think maybe build the fs before the console more butter :-)

#include "types.h"
#include "spinlock.h"
#include "fs.h"
#include "x86.h"
#include "proc.h"

static void consputc(int);

static int paniced = 0;

static struct {
	struct spinlock lock;
	int locking;
} cons;


// TODO (after build "fs", use the concept in "fs", like "inode")
static int consolewrite(struct inode *ip, char* buf, int n){
	
}

void consoleinit(){
	lockinit(&cons.lock, "console");
	devsw[CONSOLE].write = consolewrite;
	devsw[CONSOLE].read = consoleread;

	// "0" mean the frist cpu hand the IRQ(cpuid = 0)
	ioapicenable(IRQ_KBD, 0);
}
