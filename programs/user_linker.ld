ENTRY(_start)

SECTIONS
{
	. = 8M;

	.text BLOCK(16) : ALIGN(16)
	{
		*(.text)
	}

	.rodata BLOCK(16) : ALIGN(16)
	{
		*(.rodata)
	}

	.data BLOCK(16) : ALIGN(16)
	{
		*(.data)
	}

	.bss BLOCK(16) : ALIGN(16)
	{
		*(COMMON)
		*(.bss)
	}

}

