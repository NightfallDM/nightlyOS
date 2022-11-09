# jump from the boot.S, setup some env for C than jump to main
# the boot.S only load 4 sectors

.code16
.section ".hdrfield", "a"
.global hdr
# make the hdr at the end of first 512 byte region
# to see detail in 'setup.ld'
hdr:
loaded_real_addr: .word 0 # filled in "boot.S"
setup_sectors:	.byte 0 # filled in "build.c"
kernel_size:	.long 0 # filled in build system (every byte)
kernel_sectors: .byte 0 # filled in "build.c"
code32_start:	.long 0x100000
boot_flag:		.word 0xaa55

.global _start
_start:
	.byte 0xeb # "jmp" ins
	.byte setup_start-1f # this is the right usage of "0xeb" ins
1:
	.long 0 # TODO

.section "entrytext", "ax"
setup_start:
	# setup the segment reg after ljmp
	# %cs = 0x1000
	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	xorw %sp, %sp
	sti
	cld

	# setup stack
	movw $0xffff, %dx
	movzwl %dx, %esp
	
# need this because the bootloader will use %cs = %ds + xxx to jmp to
# the "_start", it means cs != ds.
	#pushw %ds
	#pushw $1f

1:
	# zero bss
	movw $__bss_start, %di
	movw $_end+3, %cx
	xorl %eax, %eax
	subw %di, %cx
	shrw $2, %cx
	rep; stosl

	# Jump to "main"
	calll main
.section "hdr_data", "a"
hdr_msg:
	.ascii "into hdr file\r\n"
	.byte 0
