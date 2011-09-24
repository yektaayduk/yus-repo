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
	/* enable clocks on peripherals */
	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_GPIOA |	// turn on GPIOA
			RCC_APB2Periph_USART1 |	// turn on USART1
			RCC_APB2Periph_AFIO,	// turn on alternate function
			ENABLE);

	// 2. GPIO configuration - configure RX and TX pin modes

	// 3. USART configuration - configure serial port settings

	// 4. finally, enable USART peripheral
}
