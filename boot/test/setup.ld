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
    .entrytext      : { *(.entrytext) }
	.inittext 		: { *(.inittext) }
	.initdata 		: { *(.initdata) }

	.text 			: { *(.text .text.*) }
	.text32 		: { *(.text32) }

	. = ALIGN(16);
	.rodata 		: { *(.rodata*) }

	. = ALIGN(16);
	.data 			: { *(.data*) }

    . = ALIGN(16);
	.bss		:
	{
		__bss_start = .;
		*(.bss)
		__bss_end = .;
	}
	. = ALIGN(16);
	_end = .;
	
	/DISCARD/		: { *(.note*) }
}
