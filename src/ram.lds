ENTRY(_start)
MEMORY {
	ram : ORIGIN = 0x20000000, LENGTH = 128K
	rom : ORIGIN = 0x08000000, LENGTH = 1M
	ccmram : ORIGIN = 0x10000000, LENGTH = 64K
}

SECTIONS {
	.text : {
		*(.isr_vectors*)
		*(.text*)
	} > ram
	
	.rodata : {
		*(.rodata*)
	} > ram

	.data : {
		*(.data*)
	} > ram

	.bss : {
		_sbss = .;
		*(.bss*) *(COMMON);
	} > ram
	_ebss = _sbss + SIZEOF(.bss);

	. = ALIGN(4);
	.init_array : {
		__init_array_start = .;
		*(.init_array*)
		__init_array_end = .;
	} > ram
}

_stack = ORIGIN(ccmram) + LENGTH(ccmram);
_stack_irq = ORIGIN(ram) + LENGTH(ram);
