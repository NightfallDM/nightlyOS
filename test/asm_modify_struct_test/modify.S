.global change;
change:
	pushq %rbx
	xorq %rbx, %rbx
	addw $111, ptest1(%rbx)
	popq %rbx
	ret
.type change, @function
