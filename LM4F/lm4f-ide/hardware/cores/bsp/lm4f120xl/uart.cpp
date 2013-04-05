
#include <stdint.h>
#include <stdarg.h>

#include <inc/hw_gpio.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_uart.h>
#include <inc/hw_types.h>
#include <driverlib/gpio.h>
#include <driverlib/interrupt.h>
#include <driverlib/rom.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>

#include "gpio.h"
#include "uart.h"

HardwareUart Serial0( 0 );
HardwareUart Serial1( 1 );
HardwareUart Serial2( 2 );
HardwareUart Serial3( 3 );
HardwareUart Serial4( 4 );
HardwareUart Serial5( 5 );
HardwareUart Serial6( 6 );
HardwareUart Serial7( 7 );

extern "C"
{
	void UART0IntHandler(void) { Serial0.ISR(&Serial0); }
	void UART1IntHandler(void) { Serial1.ISR(&Serial1); }
	void UART2IntHandler(void) { Serial2.ISR(&Serial2); }
	void UART3IntHandler(void) { Serial3.ISR(&Serial3); }
	void UART4IntHandler(void) { Serial4.ISR(&Serial4); }
	void UART5IntHandler(void) { Serial5.ISR(&Serial5); }
	void UART6IntHandler(void) { Serial6.ISR(&Serial6); }
	void UART7IntHandler(void) { Serial7.ISR(&Serial7); }
}

static const unsigned long g_UARTPeriph[] =
{
	SYSCTL_PERIPH_UART0, SYSCTL_PERIPH_UART1,
	SYSCTL_PERIPH_UART2, SYSCTL_PERIPH_UART3,
	SYSCTL_PERIPH_UART4, SYSCTL_PERIPH_UART5,
	SYSCTL_PERIPH_UART6, SYSCTL_PERIPH_UART7
};

static const unsigned long g_UARTBase[] =
{
	UART0_BASE, UART1_BASE, UART2_BASE, UART3_BASE,
	UART4_BASE, UART5_BASE, UART6_BASE, UART7_BASE
};

static const unsigned long g_UARTInt[] =
{
	INT_UART0, INT_UART1, INT_UART2, INT_UART3,
	INT_UART4, INT_UART5, INT_UART6, INT_UART7
};

HardwareUart::HardwareUart( uint8_t portNum )
{
	m_portNum = portNum;
	m_uartBase = 0;
}

void HardwareUart::begin( uint32_t baud )
{
	if(!ROM_SysCtlPeripheralPresent( g_UARTPeriph[m_portNum]) )
        return;

	m_uartBase = g_UARTBase[m_portNum];
	
	// todo
	ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	
	// Enable the UART peripheral
	ROM_SysCtlPeripheralEnable( g_UARTPeriph[m_portNum] );
	
	// Configure the UART (8-N-1)
	ROM_UARTConfigSetExpClk(m_uartBase, ROM_SysCtlClockGet(), baud,
				(UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
	
	// interrupt whenever the TX FIFO is almost empty or when any character is received
	ROM_UARTFIFOLevelSet(m_uartBase, UART_FIFO_TX1_8, UART_FIFO_RX1_8);
	ROM_UARTTxIntModeSet(m_uartBase, UART_TXINT_MODE_EOT);
	
	// clear FIFO buffers
	TxFifo.inptr = TxFifo.outptr = RxFifo.inptr = RxFifo.outptr = 0;
	
	// enable receive interrupt (initially disable transmit interrupt)
	ROM_UARTIntDisable( m_uartBase, 0xFFFFFFFF );
	ROM_UARTIntEnable( m_uartBase, UART_INT_RX | UART_INT_RT );
	ROM_IntEnable( g_UARTInt[m_portNum] );
	
	ROM_IntMasterEnable();
	TxRun = 0;
	
	// Enable the UART operation
	ROM_UARTEnable( m_uartBase );
}

void HardwareUart::end()
{
	if(!m_uartBase) return;
	
	// Disable interrupt
	ROM_UARTIntDisable( m_uartBase, 0xFFFFFFFF );
	ROM_IntDisable( g_UARTInt[m_portNum] );
	// Disable the UART operation
	ROM_UARTDisable( m_uartBase );
	// Disable the UART peripheral
	ROM_SysCtlPeripheralDisable( g_UARTPeriph[m_portNum] );	
}

void HardwareUart::putc(uint8_t c)
{
	if(!m_uartBase) return;
#if 1
	// wait until buffer has an empty slot.
	while (( (TxFifo.inptr+1) & UART_BUFF_MASK) == TxFifo.outptr)
			continue;
	//place character in buffer
	TxFifo.buff[TxFifo.inptr] = c;
	// increment in index
	TxFifo.inptr = (TxFifo.inptr + 1) & UART_BUFF_MASK;
	// start the TX sequence if not yet running
	if (!TxRun) {
		TxRun = 1;
		ROM_UARTIntEnable( m_uartBase, UART_INT_TX ); // enable TX interrupt;		
	}
#else
	ROM_UARTCharPutNonBlocking( m_uartBase, c );
#endif
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
	// checks if a character is present in the RX buffer
	return (RxFifo.inptr != RxFifo.outptr);
}

uint8_t HardwareUart::getc()
{
	uint8_t c;
	
	if(!m_uartBase) return 0;

	// wait until a character is present
	while (isrx()==0)	continue;

	// get a character from RX buffer
	c = RxFifo.buff[RxFifo.outptr];
	// increment out index
	RxFifo.outptr = (RxFifo.outptr+1)&UART_BUFF_MASK;

	return c;
}

void HardwareUart::ISR(HardwareUart *uart)
{
	uint16_t temp;
	// get and clear the interrupt status
	uint32_t status = ROM_UARTIntStatus( uart->m_uartBase, true );
	ROM_UARTIntClear( uart->m_uartBase, status );
	
	if( status & (UART_INT_RX | UART_INT_RT) )
	{
		// byte read and save to buffer
		uart->RxFifo.buff[uart->RxFifo.inptr] = UARTCharGetNonBlocking( uart->m_uartBase ) & 0xFF;
		temp = (uart->RxFifo.inptr+1) & UART_BUFF_MASK;
		if(temp != uart->RxFifo.outptr){	// avoid buffer overrun
			uart->RxFifo.inptr = temp;
		}
	}
	if( status & UART_INT_TX )
	{
		// disable the UART interrupt
		ROM_IntDisable( g_UARTInt[uart->m_portNum] );		
		// place the character to the data register
		ROM_UARTCharPutNonBlocking( uart->m_uartBase, uart->TxFifo.buff[uart->TxFifo.outptr++] );
		// reenable the UART interrupt
		ROM_IntEnable( g_UARTInt[uart->m_portNum] );
		
		uart->TxFifo.outptr &= UART_BUFF_MASK; // circular FIFO
		if(uart->TxFifo.outptr==uart->TxFifo.inptr){ // if buffer empty
			ROM_UARTIntDisable( uart->m_uartBase, UART_INT_TX ); // disable TX interrupt
			uart->TxRun = 0; // clear the flag
		}
	}
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
