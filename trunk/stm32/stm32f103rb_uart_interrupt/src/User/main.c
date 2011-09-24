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
	// TODO's
	// 1. RCC Configuration - enable peripheral clocks

	// 2. GPIO Configuration - configure RX and TX pins

	// 3. USART Configuration - serial port settings

	// 4. NVIC Configuration - enable uart interrupt

	// 5. finally, enable uart peripheral
}
