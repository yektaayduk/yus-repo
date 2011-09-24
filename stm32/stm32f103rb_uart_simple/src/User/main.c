/**********************************
 * project: STM32F10X Simple UART
 *    file: main.c
 **********************************/

#include "stm32f10x.h"

void UART1_init(void);

int main()
{
	UART1_init();

	return 0;
}

void UART1_init()
{
	// TODO
	// 1. RCC configuration - enable clocks on peripherals

	// 2. GPIO configuration - configure RX and TX pin modes

	// 3. USART configuration - configure serial port settings

	// 4. finally, enable USART peripheral
}
