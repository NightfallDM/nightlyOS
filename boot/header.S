# "setup" part

.code16
.section ".header", "a"
.global hdr
hdr:
kernel_size:	.long 0 # filled in build system (every byte)
code32_start:	.long 0x100000

.global _start
_start:
	.byte 0xeb # "jmp" ins
	.byte setup_start

.section "entrytext", "ax"
setup_start:
	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	
	# setup stack
	movw $0x2000, %ax # 8kB stack
	movw %ax, %sp
	movw $_end, %ax
	shrw $4, %ax
	movw %ax, %ss

	# zero bss
	movw $_bss_start, %di
	movw $_end+3, %cx
	xorl %eax, %eax
	subw %di, %cx
	shrw $2, %cx
	rep; stosl

	#Jump to "main"
	calll main
