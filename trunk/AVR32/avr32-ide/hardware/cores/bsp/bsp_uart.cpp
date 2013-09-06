
extern "C"
{
#include <avr32/io.h>
#include <power_clocks_lib.h>
#include <gpio.h>
#include <usart.h>
}

#include <stdint.h>
#include <stdarg.h>
#include "bsp_clock.h"
#include "bsp_uart.h"


HardwareUart Serial0( 0 );

static const gpio_map_t usart3_gpio =
{
	{AVR32_USART3_RXD_0_0_PIN, AVR32_USART3_RXD_0_0_FUNCTION},
	{AVR32_USART3_TXD_0_0_PIN, AVR32_USART3_TXD_0_0_FUNCTION}
};

HardwareUart::HardwareUart( uint8_t portNum )
{
	m_portNum = portNum;
	m_usart = NULL;
}

void HardwareUart::begin( uint32_t baud )
{
	usart_options_t m_options;
	
	if(m_portNum==0) {
		m_usart = &AVR32_USART3;
		// Assign GPIO to USART.
		gpio_enable_module(usart3_gpio, sizeof(usart3_gpio) / sizeof(usart3_gpio[0]));
	}
	//else if // todo
	//	m_usart = ...
	
	if(!m_usart) return;
	
	m_options.baudrate = baud;
	m_options.charlength = 8;
	m_options.paritytype = USART_NO_PARITY;
	m_options.stopbits = USART_1_STOPBIT;
	m_options.channelmode = USART_NORMAL_CHMODE;
  
	// Initialize USART in RS232 mode.
	usart_init_rs232(m_usart, &m_options, TARGET_PBACLK_FREQ_HZ);
			
	// clear FIFO buffers
	TxFifo.inptr = TxFifo.outptr = RxFifo.inptr = RxFifo.outptr = 0;
}

void HardwareUart::end()
{
	if(!m_usart) return;
	pcl_disable_module(AVR32_USART3_CLK_PBA);
}

void HardwareUart::putc(uint8_t c)
{
	if(!m_usart) return;
	usart_putchar(m_usart, c);
}

void HardwareUart::print(char ch)
{
	putc(ch);
}

void HardwareUart::print(int num)
{
	print("%d", num);
}

void HardwareUart::puts(const char *s)
{
	while(*s) putc(*s++);
}

uint8_t HardwareUart::isrx()
{
	if(!m_usart) return 0;
	return (m_usart->csr & AVR32_USART_CSR_RXRDY_MASK) != 0;
}

uint8_t HardwareUart::getc()
{
	if(!m_usart) return 0;

	return usart_getchar(m_usart);
}


/*------------------------------------------------------------------------/
/  Universal string handler for user console interface
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2011, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/

void HardwareUart::vprint (
	const char*	fmt,	/* Pointer to the format string */
	va_list arp			/* Pointer to arguments */
	)
{
	unsigned int r, i, j, w, f;
	unsigned long v;
	char s[16], c, d, *p;
	
	for (;;) {
		c = *fmt++;					/* Get a char */
		if (!c) break;				/* End of format? */
		if (c != '%') {				/* Pass through it if not a % sequense */
			putc(c); continue;
		}
		f = 0;
		c = *fmt++;					/* Get first char of the sequense */
		if (c == '0') {				/* Flag: '0' padded */
			f = 1; c = *fmt++;
		} else {
			if (c == '-') {			/* Flag: left justified */
				f = 2; c = *fmt++;
			}
		}
		for (w = 0; c >= '0' && c <= '9'; c = *fmt++)	/* Minimum width */
			w = w * 10 + c - '0';
		if (c == 'l' || c == 'L') {	/* Prefix: Size is long int */
			f |= 4; c = *fmt++;
		}
		if (!c) break;				/* End of format? */
		d = c;
		if (d >= 'a') d -= 0x20;
		switch (d) {				/* Type is... */
		case 'S' :					/* String */
			p = va_arg(arp, char*);
			for (j = 0; p[j]; j++) ;
			while (!(f & 2) && j++ < w) putc(' ');
			puts(p);
			while (j++ < w) putc(' ');
			continue;
		case 'C' :					/* Character */
			putc((char)va_arg(arp, int)); continue;
		case 'B' :					/* Binary */
			r = 2; break;
		case 'O' :					/* Octal */
			r = 8; break;
		case 'D' :					/* Signed decimal */
		case 'U' :					/* Unsigned decimal */
			r = 10; break;
		case 'X' :					/* Hexdecimal */
			r = 16; break;
		default:					/* Unknown type (passthrough) */
			putc(c); continue;
		}

		/* Get an argument and put it in numeral */
		v = (f & 4) ? va_arg(arp, long) : ((d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int));
		if (d == 'D' && (v & 0x80000000)) {
			v = 0 - v;
			f |= 8;
		}
		i = 0;
		do {
			d = (char)(v % r); v /= r;
			if (d > 9) d += (c == 'x') ? 0x27 : 0x07;
			s[i++] = d + '0';
		} while (v && i < sizeof(s));
		if (f & 8) s[i++] = '-';
		j = i; d = (f & 1) ? '0' : ' ';
		while (!(f & 2) && j++ < w) putc(d);
		do putc(s[--i]); while(i);
		while (j++ < w) putc(' ');
	}
}


void HardwareUart::print (			/* Put a formatted string to the default device */
	const char*	fmt,	/* Pointer to the format string */
	...					/* Optional arguments */
	)
{
	va_list arp;

	va_start(arp, fmt);
	vprint(fmt, arp);
	va_end(arp);
}

int HardwareUart::gets (		/* 0:End of stream, 1:A line arrived */
	char* buff,	/* Pointer to the buffer */
	int len		/* Buffer length */
	)
{
	int c, i;
#define	_LINE_ECHO		1
	i = 0;
	for (;;) {
		c = getc();				/* Get a char from the incoming stream */
		if (!c) return 0;			/* End of stream? */
		if (c == '\r') break;		/* End of line? */
		if (c == '\b' && i) {		/* Back space? */
			i--;
			if (_LINE_ECHO) putc(c);
			continue;
		}
		if (c >= ' ' && i < len - 1) {	/* Visible chars */
			buff[i++] = c;
			if (_LINE_ECHO) putc(c);
		}
	}
	buff[i] = 0;	/* Terminate with a \0 */
	if (_LINE_ECHO) putc('\n');
	return 1;
}
