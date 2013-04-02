#include <lm4f120xl.h>

int main(void)
{
	pinMode(LED_RED_PIN, OUTPUT);
	pinMode(LED_GREEN_PIN, OUTPUT);
	
	while(true)
	{
		setPinState(LED_RED_PIN, !getPinState(BUTTON_SW1_PIN));
		setPinState(LED_GREEN_PIN, !getPinState(BUTTON_SW2_PIN));
		delay_ms(20);
	}
}
