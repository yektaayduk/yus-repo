
#ifndef __XUART_H__
#define __XUART_H__

#include "uart.h"
#include "xprintf.h"

#define xuart_init() \
				uart_init(); \
				xdev_out(uart_putc); \
				xdev_in(uart_getc)

#endif /* __XUART_H__ */
