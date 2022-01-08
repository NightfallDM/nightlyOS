# "setup" part

.code16
.section ".hdrtext", "ax"

.section "hdrfield", "ax"
.global hdr
# make the hdr at the end of first 512 byte region
# to see detail in 'setup.ld'
.org 498
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
	# %es = %ds
	movw %ds, %ax
	movw %ax, %es
	cld
	
	# setup stack
	movw $0x2000, %dx # 8kB stack
	movzwl %dx, %esp
	movw %ax, %ss
	
# need this because the bootloader will use %cs = %ds + xxx to jmp to
# the "_start", it means cs != ds.
	pushw %ds
	pushw $1f

1:
	# zero bss
	movw $__bss_start, %di
	movw $_end+3, %cx
	xorl %eax, %eax
	subw %di, %cx
	shrw $2, %cx
	rep; stosl

	# Jump to "main"
	# calll main
