.global change;
change:
	pushq %rbx
	xorq %rbx, %rbx

	# modify test1.num2
	addw $111, (test1+4)
	#addw $111, test1+4(%rbx)

	popq %rbx
	ret
.type change, @function
