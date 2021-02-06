/*
* memcpy, memset implemention
*/
.code16
.text

# enable "-mregparam=3"
# arg order: ax, dx, cx
.global memcpy
	pushw %di
	pushw %si
	movw %ax, %di
	movw %dx, %si
	pushw %cx

	# size/4 for movsl
	shrw $2, %cx
	rep; movsl
	popw %cx

	# part less than 4 
	andw $3, %cx
	rep; movsb
	
	popw %si
	popw %di
	retl

# enable "-mregparam=3"
# arg order: ax, dx, cx
.global memset
memset:
	pushw %di
	movw %ax, %di
	movzbl %dl, %eax

	# 4 byte every step
	imull %0x01010101, %eax
	pushw %cx
	shrw $2, %cx
	rep; stosl
	popw %cx

	# part less than 4
	andw $3, %cx
	rep; stosb
	popw %di
	retl
