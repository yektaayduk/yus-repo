
#ifndef __BSP_UART_H__
#define __BSP_UART_H__

/* debug serial */
// PA0 - U0Rx
// PA1 - U0Tx

/* same pin w/ USB */
// PD4 - U6Rx
// PD5 - U6Tx

/*** J1 ***/
// 3V3
// PB5
// PB0 - U1Rx
// PB1 - U1Tx
// PE4 - U5Rx
// PE5 - U5Tx
// PB4
// PA5
// PA6
// PA7

/*** J3 ***/
// VBUS
// GND
// PD0
// PD1
// PD2
// PD3
// PE1 - U7Tx
// PE2
// PE3
// PF1 - RED_LED

/*** J4 ***/
// PF2 - BLUE_LED
// PF3 - GREEN_LED
// PB3
// PC4 - U4Rx - U1Rx (alt2)
// PC5 - U4Tx - U1Tx (alt2)
// PC6 - U3Rx
// PC7 - U3Tx
// PD6 - U2Rx
// PD7 - U2Tx
// PF4 - PUSH1

/*** J2 ***/
// GND
// PB2
// PE0 - U7Rx
// PF0 - PUSH2
// RST
// PB7
// PB6
// PA4
// PA3
// PA2

#define UART_BUFF_SIZE		256
#define	UART_BUFF_MASK		(UART_BUFF_SIZE-1)


#ifdef __cplusplus
extern "C" {
//#endif

class HardwareUart
{
public:
	HardwareUart( uint8_t portNum = 0 );
	void begin( uint32_t baud = 115200 );
	void end();
	void putc( uint8_t c );
	void print( char ch );
	void print( int num );
	void puts( const char *s );
	uint8_t isrx();
	uint8_t getc();
	
	/* from ChaN's embedded string functions */
	void print( const char *fmt, ... );
	void vprint( const char* fmt, va_list arp );
	int gets( char* buff, int len );
	
	/* interrupt service routine */
	static void ISR( HardwareUart *uart );

private:
	volatile uint8_t m_portNum;
	volatile uint32_t m_uartBase;
	volatile struct {
		uint16_t	inptr, outptr;			/* in/out index */
		uint8_t		buff[UART_BUFF_SIZE];	/* receive/transmit buffer */
	} TxFifo, RxFifo;
};

extern HardwareUart Serial0;
extern HardwareUart Serial1;
extern HardwareUart Serial2;
extern HardwareUart Serial3;
extern HardwareUart Serial4;
extern HardwareUart Serial5;
extern HardwareUart Serial6;
extern HardwareUart Serial7;

#define Serial	Serial0

//#ifdef __cplusplus
}
#endif

#endif	// __BSP_UART_H__
