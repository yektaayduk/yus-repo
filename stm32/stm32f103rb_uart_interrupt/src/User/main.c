/********************************************
 * project: STM32F10X Interrupt-Driven UART
 *    file: main.c
 ********************************************/

#include "stm32f10x.h"

void uart_init(void);

int main()
{
	uart_init();

	return 0;
}

void uart_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* enable clocks on peripherals */
	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_GPIOA |	// turn on GPIOA (for RX and TX pins)
			RCC_APB2Periph_USART1 |	// turn on USART1
			RCC_APB2Periph_AFIO,	// turn on alternate function
			ENABLE);

	// TODO: GPIO Configuration - configure RX and TX pins

	// TODO: USART Configuration - serial port settings

	// TODO: NVIC Configuration - enable uart interrupt

	// TODO: finally, enable uart peripheral
}
