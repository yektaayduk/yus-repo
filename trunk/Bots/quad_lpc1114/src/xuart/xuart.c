/*------------------------------------------------------------------------/
/  LPC1100 UART0 control module
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

#include "CoOS.h"
#include "xuart.h"

#define UART_BUFF_MASK	(UART_BUFF_SIZE	- 1)

static volatile struct {
	uint16_t	ri, wi, ct, act;
	uint8_t		buff[UART_BUFF_SIZE];
} TxBuff, RxBuff;

void UART_IRQHandler (void)
{
	uint8_t iir, d;
	int i, cnt;

	CoEnterISR();
	for (;;) {
		iir = LPC_UART->IIR;			/* Get interrupt ID */
		if (iir & 1) break;		/* Exit if there is no interrupt */
		switch (iir & 7) {
		case 4:			/* Rx FIFO is filled to trigger level or timeout occured */
			i = RxBuff.wi;
			cnt = RxBuff.ct;
			while (LPC_UART->LSR & 0x01) {		/* Get all data in the Rx FIFO */
				d = LPC_UART->RBR;
				if (cnt < UART_BUFF_SIZE) {	/* Store data if Rx buffer is not full */
					RxBuff.buff[i++] = d;
					i &= UART_BUFF_MASK;
					cnt++;
				}
			}
			RxBuff.wi = i;
			RxBuff.ct = cnt;
			break;

		case 2:			/* Tx FIFO gets empty */
			cnt = TxBuff.ct;
			if (cnt) {		/* There is one or more byte to send */
				i = TxBuff.ri;
				for (d = 16; d && cnt; d--, cnt--) {	/* Fill Tx FIFO */
					LPC_UART->THR = TxBuff.buff[i++];
					i &= UART_BUFF_MASK;
				}
				TxBuff.ri = i;
				TxBuff.ct = cnt;
			} else {
				TxBuff.act = 0; /* When no data to send, next putc() must trigger Tx sequense */
			}
			break;

		default:		/* Data error or break detected */
			LPC_UART->LSR;
			LPC_UART->RBR;
			break;
		}
	}
	CoExitISR();
}



int uart_test (void)
{
	return RxBuff.ct;
}

uint8_t uart_getc (void)
{
	uint8_t d;
	int i;

	/* Wait while Rx buffer is empty */
	while (!RxBuff.ct);

	i = RxBuff.ri;			/* Get a byte from Rx buffer */
	d = RxBuff.buff[i++];
	RxBuff.ri = i & UART_BUFF_MASK;
	__disable_irq();
	RxBuff.ct--;
	__enable_irq();

	return d;
}



void uart_putc (
	uint8_t d	/* Data byte to be sent */
)
{
	int i;

	/* Wait for Tx buffer ready */
	while (TxBuff.ct >= UART_BUFF_SIZE) ;

	__disable_irq();
	if (TxBuff.act) {
		i = TxBuff.wi;		/* Put a byte into Tx byffer */
		TxBuff.buff[i++] = d;
		TxBuff.wi = i & UART_BUFF_MASK;
		TxBuff.ct++;
	} else {
		TxBuff.act = 1;		/* Trigger Tx sequense */
		LPC_UART->THR = d;
	}
	__enable_irq();
}



void uart_init (uint32_t baudrate)
{
	uint32_t Fdiv;
	uint32_t regVal;

	NVIC_DisableIRQ(UART_IRQn);
	/* Attach UART0 to I/O pad */
	LPC_IOCON->PIO1_6 &= ~0x07;    /*  UART I/O config */
	LPC_IOCON->PIO1_6 |= 0x01;     /* UART RXD */
	LPC_IOCON->PIO1_7 &= ~0x07;
	LPC_IOCON->PIO1_7 |= 0x01;     /* UART TXD */

	/* Enable UART clock */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);
	LPC_SYSCON->UARTCLKDIV = 0x1;     /* divided by 1 */

	LPC_UART->LCR = 0x83;             /* 8 bits, no Parity, 1 Stop bit */
	regVal = LPC_SYSCON->UARTCLKDIV;

	Fdiv = (((SystemCoreClock*LPC_SYSCON->SYSAHBCLKDIV)/regVal)/16)/baudrate ;	/* baud rate */

	LPC_UART->DLM = Fdiv / 256;
	LPC_UART->DLL = Fdiv % 256;
	LPC_UART->LCR = 0x03;		/* DLAB = 0 */
	LPC_UART->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

	/* Read to clear the line status. */
	regVal = LPC_UART->LSR;

	/* Clear Tx/Rx buffers */
	TxBuff.ri = 0; TxBuff.wi = 0; TxBuff.ct = 0; TxBuff.act = 0;
	RxBuff.ri = 0; RxBuff.wi = 0; RxBuff.ct = 0;

	/* Enable Tx/Rx/Error interrupts */
	LPC_UART->IER = 0x07;
	NVIC_SetPriority(UART_IRQn, (1<<__NVIC_PRIO_BITS) - 1); // lowest priority
	NVIC_EnableIRQ(UART_IRQn);
	__enable_irq();
}


