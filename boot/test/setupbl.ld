OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(start_boot)

SECTIONS
{
	. = 0;
	.bltext		: { *(.bltext) }
}
