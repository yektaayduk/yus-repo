/*******************************
 * project: STM32 LED Blinker
 *    file: startup.s
 *******************************/
    .syntax unified
    .cpu cortex-m3
    .fpu softvfp
    .thumb

 	.section	.vector_table,"a",%progbits
	.type	g_pfnVectors, %object
	.size	g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
  .word  0x20005000    /* end of 20K SRAM */
  .word  main
