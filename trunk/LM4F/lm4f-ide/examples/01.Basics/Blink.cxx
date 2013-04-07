#include <lm4f120xl.h>

int main(void)
{
	pinMode(LED_BLUE_PIN, OUTPUT);
	
	while(true)
	{
		togglePin(LED_BLUE_PIN);
		delay_ms(500);
	}
}
