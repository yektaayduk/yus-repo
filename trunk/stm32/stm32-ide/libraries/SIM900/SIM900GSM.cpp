

#include <SIM900.h>

SIM900GSM::SIM900GSM( UartPortClass *port ) : SIM900( port )
{

}

void SIM900GSM::begin( uint32_t baud )
{
	SIM900::begin(baud);
	
	writeCommand("AT+CMGF=1");
	waitResponse("OK", 500);
}

int8_t SIM900GSM::sendSMS(const char *number, const char *message)
{
	portFlushRX();
	portWrite( "AT+CMGS=\"%s\"\r\n", number );
	if( waitResponse( ">", 5000 ) < 0 ) return (-1);
	
	portFlushRX();
	portWrite( "%s%c", message, (char)0x1A ); // append CTRL+Z
	
	return waitResponse("+CMGS", 10000);
}

int16_t SIM900GSM::hasSMS( message_status status )
{
	portFlushRX();
	switch(status)
	{
	case REC_UNREAD:
		writeCommand("AT+CMGL=\"REC UNREAD\"");
		break;
	case REC_READ:
		writeCommand("AT+CMGL=\"REC READ\"");
		break;
	case STO_UNSENT:
		writeCommand("AT+CMGL=\"STO UNSENT\"");
		break;
	case STO_SENT:
		writeCommand("AT+CMGL=\"STO SENT\"");
		break;
	case MSG_ALL:
		writeCommand("AT+CMGL=\"ALL\"");
		break;
	default:
		return (-1);
	}
	if( waitResponse("+CMGL: ", 2000) < 0 ) return (-1);
	
	return ((int16_t)getIntegerResponse());
}

int8_t SIM900GSM::deleteSMS( int16_t index, delete_flag flag  )
{
	if( index<1 || flag<DEL_INDEX || flag>DEL_ALL ) return (-1);
	portFlushRX();
	//dprint("deleteSMS %d (flag=%d)\r\n", index, flag);
	portWrite("AT+CMGD=%d,%d\r\n", index, (int)flag);
	
	return waitResponse("OK", 5000);
}

int8_t SIM900GSM::getSMS( int16_t index, char *msg_txt, int16_t msg_len, char *sender, char *timestamp )
{
	char *p1;
	char *p2;
	int16_t len;
	
	if( index<1 ) return (-1);
	
	portFlushRX();
	portWrite("AT+CMGR=%d\r\n", index);
	
	portReceiveRaw();
	//dprint("getSMS: m_rxbuff = %s\n", m_rxbuff);
	
	// format:
	//<CR><LF>+CMGR: "<message_status>","<sender>","","<timestamp>"<CR><LF><message_body><CR><LF>OK<CR><LF>
	
	// decode sender number/string
	p1 = strstr(m_rxbuff, "\",\"");
	if(!p1) return (-1);
	p2 = strstr(p1+3, "\"");
	if(p2<p1) return (-1);
	if(sender) memcpy(sender, p1+3, p2-p1-3);
	
	// decode timestamp
	p1 = strstr(p2+2, ",\"");
	if(!p1) return (-1);
	p2 = strstr(p1+2, "\"");
	if(p2<p1) return (-1);
	if(timestamp) memcpy(timestamp, p1+2, p2-p1-2);
	
	// decode message
	p1 = strstr(p2, "\r\n");
	if(!p1) return (-1);
	//len = strlen( p1+2 );
	p2 = strrchr(p1+2, 'O'); // ...<CR><LF>OK<CR><LF>
	len = (p2-3) - (p1+2);
	if(len<1) return (-1);
	if(msg_txt) memcpy(msg_txt, p1+2, msg_len<len? msg_len:len);
	
	// getSMS success!
	//if( strstr(m_rxbuff, "\r\n+CMGR: \"REC UNREAD\"") == m_rxbuff ) return REC_UNREAD;
	//if( strstr(m_rxbuff, "\r\n+CMGR: \"REC READ\"") == m_rxbuff ) return REC_READ;
	return 0;
}


