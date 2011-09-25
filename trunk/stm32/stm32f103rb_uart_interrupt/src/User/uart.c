
#include "stm32f10x.h"
#include "uart.h"

#define	UART_BUFF_MASK		(UART_BUFF_SIZE-1)
static volatile struct {
	uint16_t	inptr, outptr;			/* in/out index */
	uint8_t		buff[UART_BUFF_SIZE];	/* receive/transmit buffer */
} TxFifo, RxFifo;

void UART1_ISR(void)
{
	uint16_t c;
	if(USART_GetITStatus(UART1, USART_IT_RXNE) != RESET)
	{
		// initial loopback test
		c = USART_ReceiveData( UART1 );
		USART_SendData( UART1, c );
	}
}

void uart_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* enable clocks on peripherals */
	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_GPIOA |	// turn on GPIOA (for RX and TX pins)
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

	/* configure serial port settings */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART1, &USART_InitStructure);

	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	/* Enable the USARTz Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* clear FIFO buffers */
	TxFifo.inptr = TxFifo.outptr = RxFifo.inptr = RxFifo.outptr = 0;

	/* Enable uart receive interrupt */
	USART_ITConfig(UART1, USART_IT_RXNE, ENABLE);
	// TODO: enable transmit interrupt

	/* enable uart peripheral */
	USART_Cmd(UART1, ENABLE);
}
