
#include <SIM900.h>

SIM900GSM::SIM900GSM()
{
}

bool SIM900GSM::begin(HardwareUart *serial, uint8_t network, int pwron)
{
	return true;
}
	
bool SIM900GSM::sendSMS( const char *number, const char *message )
{
	return true;
}

bool SIM900GSM::deleteSMS( int16_t index, delete_flag flag )
{
	return true;
}

bool SIM900GSM::getSMS( int16_t index, char *msg_txt, int16_t msg_len, char *sender, char *timestamp  )
{
	return true;
}

int16_t SIM900GSM::hasSMS( message_status status )
{
	return 0;
}
