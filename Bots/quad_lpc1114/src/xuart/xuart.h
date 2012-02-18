
#ifndef __XUART_H__
#define __XUART_H__

#include "LPC11xx.h"
#include "xprintf.h"

#define UART_BUFF_SIZE	64

void uart_init (uint32_t baudrate);
int uart_test (void);
void uart_putc (uint8_t);
uint8_t uart_getc (void);

#define xuart_init(baud)	\
			uart_init(baud); \
			xdev_out(uart_putc); \
			xdev_in(uart_getc);

#define __kbhit()	uart_test()
#define __getch()	uart_getc()
#define printf		xprintf

#endif // __XUART_H__


