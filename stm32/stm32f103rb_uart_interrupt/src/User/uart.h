
#ifndef __UART_H__
#define __UART_H__

#define	UART_BUFF_SIZE	128	/* (power of 2) */

#define UART1			USART1
#define UART1_IRQn		USART1_IRQn
#define UART1_GPIO		GPIOA
#define UART1_RxPin		GPIO_Pin_10
#define UART1_TxPin		GPIO_Pin_9

#define UART1_ISR		USART1_IRQHandler

void uart_init(void);

#endif /* __UART_H__ */
