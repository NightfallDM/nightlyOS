OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
	. = 0
	.header		: {*(.header)}
	.entrytext	: {*(.entrytext)}
	.text		: {*(.text .text.*)}
	.text32		: {*(.text32)}

	. = ALIGN(16);
	.rodata		: {*(.rodata*)}

	. = ALIGN(16);
	.data		: {*(.data*)}

	. = ALIGN(16);
	.bss		:
	{
		_bss_start = .;
		*(.bss)
		_bss_end = .;
	}
	. = ALIGN(16);
	_end = .;

	/DISCARD/	: {
		*(.note*)
}
}
