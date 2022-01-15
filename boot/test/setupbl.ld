OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(boot_start)

SECTIONS
{
	. = 0;
	.bltext		: { *(.bltext) }
}
