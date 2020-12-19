ENTRY(_start)

SECTIONS
{
	. = 0x80000;

	.text : {
		KEEP(*(.boot));
		*(.text*)
	}

	.rodata ALIGN(16) : {
		*(.rodata*)
	}

	.data ALIGN(16) : {
		*(.data*)
	}

	.bss (NOLOAD) : {
		*(.bss*)
	}

	.init_array :
	{
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		PROVIDE_HIDDEN (__init_array_end = .);
	}

}
