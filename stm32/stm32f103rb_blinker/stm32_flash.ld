/*******************************
 * project: STM32 LED Blinker
 *    file: stm32_flash.ld
 *******************************/
 
/* entry point */
ENTRY(main)

/* memory areas */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 128K
    SRAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}

/* output sections */
SECTIONS
{
    /* program code into FLASH */
    .text :
    {
        *(.vector_table)    /* Vector table */
        *(.text)            /* Program code */
    } >FLASH

    /* data sections goes into SRAM */
    .data :
    {
        *(.data)            /* initialized data */
    } >SRAM
}

