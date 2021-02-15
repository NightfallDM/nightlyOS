.section ".head.text","ax"
.code32
.global startup_32
startup_32:
	cld
	cli

	# enter long mode
	lgdt gdtr64
	
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
		## TODO clear pgtable
		## PML4E
		movl $pgtable, %edi
		
		
		
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
	

.section ".pgtable", "aw",@nobits
.balign 4096
pgtable:
	# TODO
	.fill BOOT_PGT_SIZE
