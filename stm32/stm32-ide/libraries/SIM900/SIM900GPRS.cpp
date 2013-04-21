
#include <SIM900.h>

SIM900GPRS::SIM900GPRS( UartPortClass *port ) : SIM900( port )
{

}

void SIM900GPRS::begin( uint32_t baud )
{
	SIM900::begin(baud);
}
