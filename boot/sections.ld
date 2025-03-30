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
}
