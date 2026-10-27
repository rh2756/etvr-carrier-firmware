OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(reset_handler);
STACK_SIZE = 1K;

MEMORY
{
	flash (rx) : ORIGIN = 0x00000000, LENGTH = 16K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 4K
}

SECTIONS
{
	/* Interrupt vectors, user code, and read-only data. */
	.text :
	{
		. = ALIGN(4);
		KEEP(*(.vectors .vectors.*))
		*(.text .text.*)
		*(.rodata .rodata*)
		. = ALIGN(4);
	} > flash

	/* Initialized data */
	/* Gets copied from flash to ram in the startup script. */
	.data :
	{
		. = ALIGN(4);
		_sdata = .;
		*(.data .data.*)
		. = ALIGN(4);
		_edata = .;
	} > ram AT > flash

	_sidata = LOADADDR(.data); /* Address of .data section in flash memory */

	/* Uninitialized data */
	/* Gets zero'ed in the startup script. */
	.bss (NOLOAD) :
	{
		. = ALIGN(4);
		_sbss = .;
		*(.bss .bss.*)
        *(COMMON)
		. = ALIGN(4);
		_ebss = .;
	} > ram

	.stack (NOLOAD) :
	{
		. = ALIGN(8);
        _sstack = .;
        . = . + STACK_SIZE;
        . = ALIGN(8);
        _estack = .;
	} > ram

	. = ALIGN(4);
}
