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

	.bss : {
		_sbss = .;
		*(.bss*) *(COMMON);
	} > ccmram
	_ebss = _sbss + SIZEOF(.bss);

	.dma : {
		*(.dma*);
	} > ram

	. = ALIGN(4);
	.init_array : {
		__init_array_start = .;
		*(.init_array*)
		__init_array_end = .;
	} > ram

	.data : {
		*(.data*)
	} > ram
}

_stack = ORIGIN(ccmram) + LENGTH(ccmram);
_stack_irq = ORIGIN(ram) + LENGTH(ram);

_sdata = 0;
_edata = 0;
_sldata = 0;
