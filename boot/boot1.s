# 1 "boot1.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "boot1.S"
# 11 "boot1.S"
# 1 "../include/asm/e820.h" 1
# 12 "boot1.S" 2
# 1 "asm.h" 1
# 13 "boot1.S" 2
.code16
.global start

start:
 xorw %ax, %ax
 movw %ax, %es
 movw %ax, %ds
 movw %ax, %ss

# get more sector data store beside this MAR
load_loader:
 movw $0x0202, %ax
 movw $0x2, %cx
 movw $0x80, %dx
 movw $0x7e00, %bx
 int $0x13
 jc panic_spin






set_A20:
 inb $0x92, %al
 orb $0x2, %al
 outb %al, $0x92


# get_mm_info
# use e820h bios int to get the mem map;
# you can get more message in the below url
# http:
# reference from linux2.4/setup.S


# store the data at 0x0:0x400 (es:di)

get_mm_e820:

 movl $0x0, (0x400)
 xorl %ebx, %ebx
 movw $0x404, %di
jmp_e820:
 xorl %eax, %eax
 movw $0xe820, %ax
 movl $20, %ecx
 movl $0x534d4150, %edx
 int $0x15
 jc bad_e820
 cmpl $0x534d4150, %eax
 jne bad_e820
 cmpl $1, 16(%di)



 jne again_e820 # do not count,do not add the 0x400
good_e820:
 movl (0x400), %eax
 cmpl $32, %eax
 jnl mm_map_finish
 incl (0x400) # count
 movw %di, %ax
 add $20, %ax
 movw %ax, %di
again_e820:
 cmpl $0x0, %ebx
 jne jmp_e820

mm_map_finish:
 jmp switch_protect

bad_e820:
 addl $100, (0x400) # if err to read,then add 100,so you can check
 jmp mm88

mm88:
 movw $0x8800, %ax
 int $0x15
 movw %ax, (0x404 +32*20) # after the 0x404, xxx KByte(2-Byte)



switch_protect:
 lgdt gdtr_value # contain the value should load into gdtr

 cli
 movl %cr0, %eax
 orl $0x1, %eax
 movl %eax, %cr0







 ljmp $(1<<3), $switch2



#.code32
#start32:

# movw $0x1000, %ax
# movw %ax, %ds
# movw %ax, %es
# movw %ax, %ss
# movw $0x0, %ax
# movw %ax, %fs
# movw %ax, %gs



# movl $start, %esp
# jmp 0x7e00 # the address behind this program

# panic and spin
panic_spin:
 jmp panic_spin

# switch to "long mode-64 mode"
.code32
switch2:
 movl $(1<<20), %eax
 movl %eax, %cr3

 movb $0x23, (0x100000)
 movw $0x1010, (0x100001)
 movw $0xc3, (0x101000)
 movl $0x0, (0x101003)

 movl %cr4, %eax
 orl $(1<<5), %eax
 movl %eax, %cr4

 movl $0xc0000080, %eax
 rdmsr
 orl $(1<<8), %eax
 wrmsr

 movl %cr0, %eax
 orl $(1<<31), %eax
 movl %eax, %cr0

enter_64:
# movl $0x100000, %eax
# movl %eax, %esp
 lgdt GDTR_64V
 ljmp $(1<<3), $mode_64
.code64
mode_64:
 movw $(0x2<<3), %ax
 movw %ax, %ds
 movw %ax, %es
 movw %ax, %fs
 movw %ax, %gs
 movw $(0x2<<3), %ax # can't set ss point the code page
 movw %ax, %ss
 movq $0x1000000, %rax
 movq %rax, %rsp
 callq loadmain
spin64:
 jmp spin64




.p2align 2
gdt:
 .word 0, 0; .byte 0, 0, 0, 0 # null
 .word (((0xffffffff)>>12)&0xffff), ((0x0)&0xffff); .byte (((0x0)>>16)&0xff), ((0x8|0x2)|0x90), ((((0xffffffff)>>28)&0xf)|0xc0), (((0x0)>>24)&0xff) # code seg base:0x0 limit:4G
 .word (((0xffffffff)>>12)&0xffff), ((0x0)&0xffff); .byte (((0x0)>>16)&0xff), ((0x0|0x2)|0x90), ((((0xffffffff)>>28)&0xf)|0xc0), (((0x0)>>24)&0xff) # data seg base:0x0 limit:4G


gdtr_value:
 .word (gdtr_value - gdt -1) #"-1" need because this ins bigger "1"
 .long gdt #address gdt

.p2align 2
gdt64:
 .word 0, 0; .byte 0, 0, 0, 0
 .word 0x0
 .word 0x0
 .byte 0x0
 .byte 0x9b
 .byte 0xaf
 .byte 0x0

 .word 0x0
 .word 0x0
 .byte 0x0
 .byte 0x93
 .byte 0x1
 .byte 0x0
GDTR_64V:
 .word (GDTR_64V - gdt64 -1)
 .long gdt64



.org 510
end:
 .word 0xaa55
