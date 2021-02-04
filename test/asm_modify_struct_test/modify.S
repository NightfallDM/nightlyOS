.global change;
change:
	pushq %rbx
	xorq %rbx, %rbx
	addw $111, (test1)
	#addw $111, test1(%rbx)
	popq %rbx
	ret
.type change, @function
