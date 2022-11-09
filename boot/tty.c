static void __attribute__((section(".initext"))) bios_putchar(const char ch) {
	unsigned short ax_val = (0xe << 8) | ch;
	asm("pushal\n\t"
		"pushfl\n\t"
		"movw $0x7, %%bx\n\t"
		"movw $1, %%cx\n\t"
		"xorw %%ax, %%ax\n\t"
		"movw %0, %%ax\n\t"
		"int $0x10\n\t"
		"popfl\n\t"
		"popal"
		:
		: "m"(ax_val)
		);
}

void __attribute__((section(".inittext"))) putchar(const char ch) {
	if (ch == '\n')
		putchar('\r');
	bios_putchar(ch);	
}

void __attribute__((section(".inittext"))) puts(const char* str) {
	while (*str)
		putchar(*str++);
}
