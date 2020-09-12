#include "mmu.h"
# TODO alltraps

.global trapret
trapret:
	popal
	popl %gs
	popl %fs
	popl %es
	popl %ds
	addl $0x8, %esp # trapno and errcode
	iret # pop "eip" and "cs".
