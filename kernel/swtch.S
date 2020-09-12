# Context switch

# void swtch(struct context **old, struct context *new);

# Save the current registers on the stack, creating a struct
# context, and save its address in *old.
# Switch stacks to new and pop previously-saved registers.

.global swtch
swtch	
	# call the "swtch" will pop "new", "old" and "ret_address"(eip)
	movl 4(%esp), %eax # 4(%esp) mean the "old" context(&&oldcontext).
	movl 8(%esp), %edx # 8(%esp) mean the "new" context(&&newcontext).

	# Save old calles-saved registers in current stack
	pushl %ebp
	pushl %ebx
	pushl %esi
	pushl %edi
	# Note: eip have already push by call the "swtch".
	# just at (%esp).

	# Switch stacks
	movl %esp, (%eax) # save the old context in current stack
	# just like "*old = esp".

	movl %edx, %esp # load the new context


	# load new caller-saved registers
	# careful the order.
	popl %edi
	popl %esi
	popl %ebx
	popl %ebp
	
	# Now the esp point to the "eip" that we should get to 
	# run, the "ret" ins pop the "eip"(on stack) to the eip(register)
	# So, the process swtch to another.
	ret
