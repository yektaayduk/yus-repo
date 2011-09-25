/********************************************
 * project: STM32F10X Interrupt-Driven UART
 *    file: main.c
 ********************************************/

#include "stm32f10x.h"
#include "uart.h"

int main()
{
	uart_init();

	uart_puts("hello stm32!\r\n");

	while(1)
	{
	}
	return 0;
}

