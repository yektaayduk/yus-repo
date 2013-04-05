/*******************************************************************
 *
 * lm4f120xl.ld - Linker configuration file for LM4F-IDE projects.
 *
 *******************************************************************/

/* Entry Point */
ENTRY(ResetISR)

/* Generate a link error if heap and stack don't fit into RAM */
_minimum_stack_size = 0x100; /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    RAM (rwx)  : ORIGIN = 0x20000000, LENGTH = 32K
}

/* Define output sections */
SECTIONS
{

	/* The program code and other data goes into FLASH */
	.text :
	{
		. = ALIGN(0x80);
		KEEP(*(.isr_vector))
		. = ALIGN(4);
		CREATE_OBJECT_SYMBOLS
		*(.text .text.* .gnu.linkonce.t.*)
		*(.plt)
		*(.gnu.warning)
		*(.glue_7t) *(.glue_7) *(.vfp11_veneer)
		*(.ARM.extab* .gnu.linkonce.armextab.*)
		*(.gcc_except_table)
	} >FLASH

	/* .ARM.exidx */
	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} >FLASH
	__exidx_end = .;

	.rodata : ALIGN (4)
	{
		*(.rodata .rodata.* .gnu.linkonce.r.*)
		
		. = ALIGN(4);
		KEEP(*(.init))
		
		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;
		
		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;
		
		. = ALIGN(4);
		KEEP(*(.fini))
		
		. = ALIGN(4);
		__fini_array_start = .;
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		__fini_array_end = .;
		
		. = ALIGN(0x4);
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*crtend.o(.ctors))
		
		. = ALIGN(0x4);
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*crtend.o(.dtors))
		
		*(.init .init.*)
		*(.fini .fini.*)
		
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		PROVIDE_HIDDEN (__fini_array_end = .);
		
		. = ALIGN (8);
		*(.rom)
		*(.rom.b)
		_etext = .;
		_sidata = _etext;
	} >FLASH

	/* initialized data section  */
	.data : ALIGN (8)
	{
		_data = . ;		
		. = ALIGN(4);
		KEEP(*(.jcr))
		*(.got.plt) *(.got)
		*(.shdata)
		*(.data .data.* .gnu.linkonce.d.*)
		. = ALIGN (8);
		*(.ram)
		*(.ramfunc*)
		 . = ALIGN(4);
		_edata = .;
	} >RAM AT>FLASH

	/* uninitialized data section */
	.bss (NOLOAD):
	{
		. = ALIGN(4);
		_bss = .;
		*(.shbss)
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(COMMON)
		. = ALIGN (8);
		*(.ram.b)
		. = ALIGN(4);
		 _ebss = . ;
		_end = .;
		__end = .;
	} >RAM AT>FLASH

    /* user stack section */
    ._usrstack (NOLOAD):
    {
        . = ALIGN(4);
        _susrstack = . ;
        . = . + _minimum_stack_size ;
        . = ALIGN(4);
        _eusrstack = . ;
    } >RAM
}

/* end of allocated ram is start of heap, heap grows up towards stack*/
PROVIDE(end = _end);

/* top of stack starts at end of ram, stack grows down towards heap */
PROVIDE (_estack = ORIGIN(RAM) + LENGTH(RAM));
