
#include <stdint.h>
#include "timer.h"

void delaytimer_init(void)
{
	// todo: setup free running timers (1MHz & 1kHz)
}

void delay_us(uint16_t count) // fix me
{
	volatile unsigned long cnt;
	for(cnt = 0; cnt < 0x800; cnt++) {}
}

void delay_ms(uint16_t count) // fix me
{
	volatile unsigned long cnt;
	for(cnt = 0; cnt < 0x1FFFFF; cnt++) {}
}
