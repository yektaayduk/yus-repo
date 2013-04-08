/*
MultiSerial
receives from serial port 1, sends to the debug serial (Serial 0)
*/

#include <lm4f120xl.h>

int main(void)
{
	Serial.begin(9600);
	Serial1.begin(9600);
	
	while(true)
	{
		if (Serial1.isrx()){
			unsigned char byte = Serial1.getc();
			Serial.putc(byte); 
		}
	}
}
