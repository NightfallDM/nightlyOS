# Now we are in the protected mode
# So, switch to the long mode right now

# The Order(protected mode 2 long mode)(with PAGE)

# Note that the "PAGE" is needed when you get into the long mode
# because the long mode use the "flat segment".
# It means that there is not segment protected ,So the "PAGE"
# protected mechanism is needed.

# 1. starting from protected mode, disable paging
# 2. enable "PAE"
# 3. load "PML4/5"(LEVEL 4/5 page map table) into CR3
# 4. enable IA-32e mode by setting IA32_EFER.LME = 1
# 5. enable paging

#include"setup.h"
.code32
.global set_long_mode
set_long_mode:
	# enable "PAE"
	movl %cr4, %eax
	xorl $0x20, %eax
	movl %eax, %cr4

load_cr3:
	# 0x10000 -- PML4T(physical address)
	movl $0x10000, %edi
	movl %edi, %cr3
set_pv_page_map:
	PML4E(0x11000)
	PDPTE(0x12000)
	# PDE(0x13000)
	# PTE(0x0)
	movl $0xc0000080, %ecx
	rdmsr
	xor $0x100, %eax
	wrmsr
# enable paging to get into the long mode, compatibility mode.
enable_paging:
	movl %cr0, %eax
	xorl $(1<<31), %eax
	movl %eax, %cr0

	lgdt LGDT64
	ljmp $(1<<3), $real_long_mode

# set GDT64
.p2align 2
GDT64:
	.quad 0x0
	// Code seg:
	.long 0x0
	.byte 0x0
	.byte 0x9a
	.byte 0xaf
	.byte 0x0

	// Data seg:
	.long 0x0
	.byte 0x0
	.byte 0x92
	.word 0x0

LGDT64:
	.word (LGDT64 - GDT64 -1)
	.long GDT64

	
