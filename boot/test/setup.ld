OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
	. = 0;
	.hdrtext		: { *(.hdrtext) }
    . = 498;
    .hdrfield       : { *(.hdrfield) }
	.hdr_data		: { *(.hdr_data) }
    .entrytext      : { *(entrytext) }

    . = ALIGN(16);
	.bss		:
	{
		__bss_start = .;
		*(.bss)
		__bss_end = .;
	}
	. = ALIGN(16);
	_end = .;
}
