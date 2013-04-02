#include <lm4f120xl.h>

int main(void)
{
	pinMode(LED_RED_PIN, OUTPUT);
	pinMode(LED_GREEN_PIN, OUTPUT);
	pinMode(LED_BLUE_PIN, OUTPUT);
	
	while(true)
	{
		setPinHigh(LED_RED_PIN);
		//setPinLow(LED_GREEN_PIN);
		setPinLow(LED_BLUE_PIN);
		delay_ms(1000);
		
		setPinLow(LED_RED_PIN);
		setPinHigh(LED_GREEN_PIN);
		//setPinLow(LED_BLUE_PIN);
		delay_ms(1000);
		
		//setPinLow(LED_RED_PIN);
		setPinLow(LED_GREEN_PIN);
		setPinHigh(LED_BLUE_PIN);
		delay_ms(1000);
	}
}
