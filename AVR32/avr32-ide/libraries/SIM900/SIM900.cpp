
#include <SIM900.h>

SIM900::SIM900():
	m_ser(NULL),
	m_net(NET_SMART),
	m_pwron(-1)
{
}
bool SIM900::begin(HardwareUart *serial, uint8_t network, int pwron)
{
	return true;
}

bool SIM900::powerOn()
{
	return true;
}

bool SIM900::powerOff()
{
	return true;
}
	
void SIM900::flushInput()
{

}

bool SIM900::sendATcmd(const char *cmd, const char *resp, uint32_t timeout, uint8_t retry, bool clearRX)
{
	return true;
}
	
