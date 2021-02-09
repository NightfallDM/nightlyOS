.global _start
_start:
	# can not use in 64bit addressing model
	#movq (symbol), %rax

	# rip relative addressing work on the first command, the second don't work
	leaq symbol(%rip), %rax
	#leaq 1000(%rip), %rax

	# can use in 32 bit addressing model(e.g not rip relative addressing)
	#leal (symbol), %eax

.org 1000
symbol:
	.quad 2000
