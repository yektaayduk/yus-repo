/* Blink without Delay
  Turns on and off an LED without using the delay() function.
 */

#include <lm4f120xl.h>

unsigned long previousMillis = 0;	// will store last time LED was updated
unsigned long interval = 500;		// interval at which to blink (milliseconds)

int main(void)
{
	pinMode(LED_GREEN_PIN, OUTPUT);
	
	while(true)
	{
		unsigned long currentMillis = millis();
		if(currentMillis - previousMillis > interval) {
			togglePin(LED_GREEN_PIN);
			previousMillis = currentMillis;
		}
	}
}
