/**********************************
 * project: STM32F10X Simple UART
 *    file: main.c
 **********************************/

#include "stm32f10x.h"

#define UART1_GPIO		GPIOA
#define UART1_RxPin		GPIO_Pin_10
#define UART1_TxPin		GPIO_Pin_9

void UART1_init(void);

int main()
{
	UART1_init();

	return 0;
}

void UART1_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* enable clocks on peripherals */
	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_GPIOA |	// turn on GPIOA
			RCC_APB2Periph_USART1 |	// turn on USART1
			RCC_APB2Periph_AFIO,	// turn on alternate function
			ENABLE);

	/* Configure RX pin as input floating */
	GPIO_InitStructure.GPIO_Pin = UART1_RxPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART1_GPIO, &GPIO_InitStructure);
	/* Configure TX pin as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = UART1_TxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UART1_GPIO, &GPIO_InitStructure);

	// 3. USART configuration - configure serial port settings

	// 4. finally, enable USART peripheral
}
