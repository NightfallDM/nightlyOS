.code16
.text

# should compile with -mregparm=3
# this will change es content
# so if you need es to be ori,you should store it yourself
.global memcpy_es
memcpy_es:
	popw %es
	pushw %si
	pushw %di
	movw %ax, %di
	movw %dx, %si
	pushw %cx
	shrw $2, %cx
	rep; movsl
	popw %cx
	andw $3, %cx
	rep; movsb
	popw %di
	popw %si
	retl

.global memcpy
memcpy:
	pushw %si
	pushw %di
	movw %ax, %di
	movw %dx, %si
	pushw %cx
	shrw $2, %cx
	rep; movsl
	popw %cx
	andw $3, %cx
	rep; movsb
	popw %di
	popw %si
	retl

