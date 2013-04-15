#include <platform.h>

int main(void)
{
	Serial.begin(115200);
	
	pinMode(PA0, INPUT_PULLUP); // digital input pin2
	pinMode(LED_PIN, OUTPUT);
    
	while(true)
	{
	    bool sensorVal = getPinState(PA0);
		Serial.print("%d\n", sensorVal);
		if(sensorVal)
			setPinHigh(LED_PIN);
		else
			setPinLow(LED_PIN);
	}
}
