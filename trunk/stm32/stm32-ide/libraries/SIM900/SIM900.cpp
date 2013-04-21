
#include <SIM900.h>
#include <platform.h> // millis()

SIM900::SIM900( UartPortClass *port )
{
	portSetPort( port );
}

void SIM900::begin( uint32_t baud )
{
	portSetBaud( baud );
	
	writeCommand("AT");
	waitResponse("OK", 200);
	// reset factory settings
	writeCommand("AT&F");
	waitResponse("OK", 5000);
	// switch off echo
	writeCommand("ATE0");
	waitResponse("OK", 200);
}

int8_t SIM900::writeCommand( const char *cmd, bool bCRLF, bool bFlushRX )
{
	if(bFlushRX) portFlushRX();
	
	//dprint("writeCommand( %s )\n", cmd);
	portWrite(cmd);
	if(bCRLF) portWrite("\r\n");
	
	return 0;
}

int32_t SIM900::portReceiveRaw( const char *str_to_wait, uint16_t timeout, bool bInt )
{
	char *p = 0;
	int8_t ch;
	uint16_t i, ms;
	
	memset(m_rxbuff, '\0', RX_BUFF_SIZE);
	i = 0; ms = millis();
	do{
		if( portHasChar() ){
			ch = portGetChar();
			if( bInt && (ch<'0' || ch>'9') ) break;
			m_rxbuff[i++] = ch;
			if(str_to_wait){
				p = strstr(m_rxbuff, str_to_wait);
				if( p>0 ) return 0; // string to be checked was found
			}
			ms = millis();
		}
	}while( (i<RX_BUFF_SIZE-1) && ((uint16_t)(millis()-ms)<timeout) );
	//m_rxbuff[i] = '\0';
	
	//dprint("m_rxbuff = %s [i = %d, str_to_wait = %s, p = %x]\n", m_rxbuff, i, str_to_wait, p);
	if(bInt) return atoi(m_rxbuff); // decoded integer
	if(i<RX_BUFF_SIZE-1) return (-1); // timeout
	return (-2); // rx buffer full
}

