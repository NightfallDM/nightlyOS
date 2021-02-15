.section ".head.text","ax"
.code32
.global startup_32
startup_32:
	cld
	cli

	# enter long mode
	lgdt gdtr64

	# setup a stack
	movl $0x100000, %ebx # default load addr for startup_32
	leal boot_stack_end(%ebx), %esp
	
	# reload segment registers
	movl $0x10, %eax # __KERNEL_DS(selector)
	movl %eax, %ds
	movl %eax, %es
	movl %eax, %fs
	movl %eax, %gs
	movl %eax, %ss

# enter 64 bit mode
	# Enable "PAE"
	movl %cr4, %eax
	orl $0x20, %eax
	movl %eax, cr4

	# early boot 4G pagetable
		# Build Lever 4

		## clear pgtable
		movl $0x100000, %ebx
		leal pgtable(%ebx), %edi
		xorl %eax, %eax
		movl $(6*4096/4), %ecx
		rep stosl

		## PML4E (1)
		leal pgtable(%ebx), %edi
		leal 0x1007(%edi), %eax
		movl %eax, 0(%edi)

		##  PDPTE (4)
		leal pgtable+0x1000(%ebx), %edi
		leal 0x1007(%edi), %eax
		movl $4, %ecx # need 4 PDPTE
1:		movl %eax, (%edi)
		addl $0x1000, %eax	# 0x1000 byte every page
		addl $8, %edi	# 8 byte every PDPTE
		decl %ecx
		jnz 1b

		## PDE (512*4)
		leal pgtable+0x2000(%ebx), %edi
		movl $0x183, %eax
		movl $2048, %ecx # need 2048 PDE
1:		movl %eax, 0(edi)
		addl $0x200000, %eax # physical addr 2M page
		addl $8, %edi
		decl %ecx
		jnz 1b

	# Enable early boot page tables
	leal pgtable(%ebx), %eax
	movl %eax, %cr3
	
	# Enable Long mode in EFER
	movl $0xc0000080, %eax
	rdmsr
	orl $(1<<8), %eax
	wrmsr

	# lret to enter 64 bit mode
	leal startup_64(%ebx), %eax
	pushl $(1<<3)
	pushl %eax
	movl %cr0, %eax
	orl $(1<<31), %eax
	movl %eax, %cr0
	lret	
	
.code64
.org 0x200
startup_64:
	xorl %eax, %eax
	movl %eax, %ds
	movl %eax, %es
	movl %eax, %ss
	movl %eax, %fs
	movl %eax, %gs
	# TODO	
.data
.balign 8
gdt64:
	.quad 0
	.quad 0x00cf9a000000ffff	# __KERNEL32_CS
	.quad 0x00af9a000000ffff	# __KERNEL_CS
	.quad 0x00cf92000000ffff	# __KERNEL_DS
	.quad 0x0080890000000000	# TS description
	.quad 0x0			# don't know

gdtr64:
	.word gdtr64 - gdt64 - 1
	.long gdt64
	
.bss
.balign 4
boot_heap:	.fill 0x10000, 1, 0
boot_stack:	.fill 0x4000, 1, 0
.balign 16
boot_stack_end: .byte 0xaa

.section ".pgtable", "aw",@nobits
.balign 4096
pgtable:
	# TODO
	.fill 6*4096, 1, 0
