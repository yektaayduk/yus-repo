/********************************************
 * project: STM32F10X xprintf UART
 *    file: main.c
 ********************************************/

#include "stm32f10x.h"
#include "xuart.h"

int main()
{
	xuart_init(115200);

	xprintf("hello stm%d!\r\n", 32);

	while(1)
	{
		if(uart_isrx()){
			uart_putc( uart_getc() ); // loopback
		}
		/* do other tasks here */
	}
	return 0;
}

