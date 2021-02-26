ENTRY(_start)

SECTIONS
{
	. = 0x80000;

	. = ALIGN(4096);
	__TEXT_START = .;
	.text : {
		KEEP(*(.boot));
		*(.text*)
	}
	__TEXT_END = .;

	. = ALIGN(4096);
	__RODATA_START = .;
	.rodata ALIGN(4K) : {
		*(.rodata*)
	}
	__RODATA_END = .;

	. = ALIGN(4096);
	__DATA_START = .;
	.data ALIGN(4K) : {
		*(.data*)
	}
	__DATA_END = .;

	. = ALIGN(4096);
	__BSS_START = .;
	.bss (NOLOAD) : {
		*(.bss*)
	}
	__BSS_END = .;

	. = ALIGN(4096);
	__APP_TEXT_START = .;
	.app.text : {
		*(app.text*)
	}
	__APP_TEXT_END = .;

	. = ALIGN(4096);
	__APP_RODATA_START = .;
	.app.rodata ALIGN(4K) : {
		*(app.rodata*)
	}
	__APP_RODATA_END = .;

	. = ALIGN(4096);
	__APP_DATA_START = .;
	.app.data ALIGN(4K) : {
		*(app.data*)
	}
	__APP_DATA_END = .;

	. = ALIGN(4096);
	__APP_BSS_START = .;
	.app.bss (NOLOAD) : {
		*(app.bss*)
	}
	__APP_BSS_END = .;

	.init_array :
	{
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		PROVIDE_HIDDEN (__init_array_end = .);
	}

	. = ALIGN(4096);
	__MAP_START = .;
	map ALIGN(4K) : {
		*(.data*);
		*(.bss*);
		*(.rodata*);
	}
}
