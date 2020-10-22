# This is the implemention of the "sbi.h"

# Because i do not want to use the inline assembly to write these code,
# So, I just use origin asm code

# The asm syntax and calling convention reference from:
# 	https://github.com/riscv/riscv-asm-manual/blob/master/riscv-asm.md
#	https://github.com/riscv/riscv-elf-psabi-doc/blob/master/riscv-elf.md


# extern void sbi_console_putchar(int ch);
.global sbi_console_putchar
sbi_console_putchar:
	li a7, 0x01
	li a6, 0x0
	ecall
	li a0, 0x0
	ret
