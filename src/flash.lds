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
	} >rom
	
	.rodata : {
		*(.rodata*)
	} >rom

	.bss : {
		_sbss = .;
		*(.bss*) *(COMMON);
	} > ram
	_ebss = _sbss + SIZEOF(.bss);

	.dma : {
		*(.dma*);
	} > ram

	. = ALIGN(4);
	.init_array : {
		__init_array_start = .;
		*(.init_array*)
		__init_array_end = .;
	} > rom

	.data : {
		*(.data*)
	} >ram AT>rom
}

_stack = ORIGIN(ccmram) + LENGTH(ccmram);
_stack_irq = ORIGIN(ram) + LENGTH(ram);

_sdata = LOADADDR(.data);
_edata = LOADADDR(.data)+SIZEOF(.data);
_sldata = ADDR(.data);
