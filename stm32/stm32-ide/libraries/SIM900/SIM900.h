
#ifndef __SIM900_H
#define __SIM900_H

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include <HardwareSerial.h>

#define dprint( str, args... )		Serial.print( str, ##args )

class SIM900
{
protected:	
	#define RX_BUFF_SIZE	1024 //256
	char m_rxbuff[RX_BUFF_SIZE];
	
	#define UartPortClass				HardwareSerial
	#define DefaultUartPort				Serial2
	UartPortClass *m_port;
	#define portSetPort(p)				{ m_port = p; }
	#define portSetBaud(b)				m_port->begin(b)
	#define portHasChar()				m_port->isrx()
	#define portGetChar()				m_port->getc()
	#define portWrite( str, args... )	m_port->print( str, ##args )
	
public:
	SIM900( UartPortClass *port = &DefaultUartPort );
	void begin( uint32_t baud = 9600 );
	
	int8_t writeCommand( const char *cmd = "AT", bool bCRLF = true, bool bFlushRX = true  );
	int32_t portReceiveRaw( const char *str_to_wait = NULL, uint16_t timeout = 2000, bool bInt = false );
	#define portFlushRX()			portReceiveRaw( NULL, 500, false )
	#define getIntegerResponse()	portReceiveRaw( NULL, 1000, true )
	#define waitResponse(rsp, to)	portReceiveRaw( rsp, to, false )
};


class SIM900GSM : public SIM900
{
public:
	enum message_status{
		REC_UNREAD = 1,
		REC_READ = 2,
		STO_UNSENT = 3,
		STO_SENT = 4,
		MSG_ALL = 5
	};
	enum delete_flag{
		DEL_INDEX = 0,
		DEL_ALL_READ = 1,
		DEL_ALL = 4
	};

	SIM900GSM( UartPortClass *port = &DefaultUartPort );
	virtual void begin( uint32_t baud = 9600 );
	
	// SMS
	int8_t sendSMS( const char *number, const char *message );
	int16_t hasSMS( message_status status = MSG_ALL );
	int8_t deleteSMS( int16_t index = 1, delete_flag flag = DEL_INDEX );
	int8_t getSMS( int16_t index, char *msg_txt, int16_t msg_len, char *sender = NULL, char *timestamp = NULL );
};


class SIM900GPRS : public SIM900
{
public:
	SIM900GPRS( UartPortClass *port = &DefaultUartPort );
	virtual void begin( uint32_t baud = 9600 );
};


#endif // __SIM900_H
