#include <platform.h>

int main(void)
{
	pinMode(LED_PIN, OUTPUT);
	setPinLow(LED_PIN);
	while(1)
	{
		togglePin(LED_PIN);
		delay_ms(500);
	}
}
