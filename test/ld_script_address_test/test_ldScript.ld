ENTRY(main)

SECTIONS
{
	. = 0;
	.text	:{*(.text)}
	.bss	:{*(.bss*)}	
	/DISCARD/:{*(.note* .comment*)}
}
