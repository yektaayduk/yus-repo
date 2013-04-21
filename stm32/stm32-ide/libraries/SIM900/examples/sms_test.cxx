#include <platform.h>
#include <SIM900.h>

SIM900GSM gsm;
char smsbuffer[200];
char sender[13+1];
char time[20+1];
void blink(void);

int main(void)
{
	Serial.begin(115200);
	gsm.begin(/*9600*/);
	pinMode(LED_PIN, OUTPUT);
	Serial.print("SIM900 SMS demo\n");
  #if 1
	//gsm.sendSMS("+639123456789", "hello world!");
	while(gsm.sendSMS("214", "?1515")<0) // e.g. balance inquiry (SMART)
	{
		blink();
	}
  #endif
	while(true)
	{
		int idx = gsm.hasSMS();
		if(idx>0){
			Serial.print("[sms index = %d]\n", idx);
			memset(smsbuffer, 0, sizeof(smsbuffer));
			memset(sender, 0, sizeof(sender));
			memset(time, 0, sizeof(time));
			//gsm.getSMS(idx, smsbuffer, sizeof(smsbuffer) /*, NULL, NULL */);
			if(gsm.getSMS(idx, smsbuffer, sizeof(smsbuffer), sender, time)>=0)
			{
				Serial.print("sender: %s\n", sender);
				Serial.print("time: %s\n", time);
				Serial.print("msg: %s\n", smsbuffer);
				gsm.deleteSMS(idx);
			}			
		}
		blink();
	}
}

void blink(void)
{
	setPinHigh(LED_PIN);
	delay_ms(500);
	setPinLow(LED_PIN);
	delay_ms(2500);
}

