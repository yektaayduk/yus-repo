
#include <SIM900.h>

SIM900GSM::SIM900GSM()
{
}

bool SIM900GSM::begin(HardwareUart *serial, uint8_t network, int pwron)
{
	if( !SIM900::begin(serial, network, pwron) ) return false;
	
	if(!sendATcmd("AT+CMGF=1", "OK")) return false; // text mode
	gsmdebug("sms init: AT+CMGF=1 -> ok\r\n");
	
	return true;
}
	
bool SIM900GSM::sendSMS( const char *number, const char *message )
{
	gsmdebug("gsm send sms...\r\n");

	m_ser->print("AT+CMGS=\"");
	m_ser->print(number);
	if(!sendATcmd("\"", ">", 5000, 1)) return false;
	gsmdebug("send sms: begin message\r\n");
	
	m_ser->print(message);
	m_ser->print((char)0x1A ); // append CTRL+Z
	gsmdebug("send sms: end message\r\n");
	
	uint32_t prev_millis = millis();
	while( !m_ser->isrx() && (millis()-prev_millis<10000)); // wait for a character
	if( !m_ser->isrx() ) return false; // timeout occurred
	
	if( waitResponse("+CMGS", DEFAULT_SIM900_TIMEOUT) < 0 ) return false;
	gsmdebug("send sms OK\r\n");
	return true;
}

bool SIM900GSM::deleteSMS( int16_t index, delete_flag flag )
{
	gsmdebug("gsm delete sms...\r\n");
	
	if( index<1 || flag<DEL_INDEX || flag>DEL_ALL ) return false;
	
	m_ser->print("AT+CMGD=%d,%d", index, (int)flag);
	
	if(!sendATcmd("", "OK", 5000, 1)) return false;
	
	gsmdebug("delete sms index[%d]\r\n", index);	
	return true;
}

bool SIM900GSM::getSMS( int16_t index, char *msg_txt, int16_t msg_len, char *sender, char *timestamp  )
{
	// todo
	return true;
}

int16_t SIM900GSM::hasSMS( message_status status )
{
	// todo
	return 0;
}
