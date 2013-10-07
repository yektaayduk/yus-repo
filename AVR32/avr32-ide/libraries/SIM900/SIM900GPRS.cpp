
#include <SIM900.h>

SIM900GPRS::SIM900GPRS()
{
}

bool SIM900GPRS::begin(HardwareUart *serial, uint8_t network, int pwron)
{
	return true;
}
	
bool SIM900GPRS::configGPRS(uint8_t network)
{
	return true;
}

bool SIM900GPRS::openGPRS()
{
	return true;
}

bool SIM900GPRS::closeGPRS()
{
	return true;
}

bool SIM900GPRS::isOpenedGPRS()
{
	return true;
}
	
int SIM900GPRS::httpget(const char *url, const char *path, char *response, int len, bool endGPRS)
{
	return 0;
}
