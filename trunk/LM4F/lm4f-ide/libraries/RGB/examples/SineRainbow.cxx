/*
    http://basecase.org/env/on-rainbows
*/

#include <lm4f120xl.h>
#include <math.h>
#include <RGB.h>

volatile unsigned long g_ulColors[3];

static void sinebow(int angle)
{
	float fRed, fGreen, fBlue;
	float fTemp;
	float rad = (float)angle * M_PI / 180.0f;

	fRed = sinf(rad + M_PI_2) + 1.0f; // -90
	fGreen = sinf(rad - M_PI_2/3) + 1.0f; // -30
	fBlue = sinf(rad - M_PI_2 - M_PI/3) + 1.0f; // -150
	
	fTemp = 16383 * fRed * RED_WHITE_BALANCE;
	g_ulColors[RED] = (unsigned long) fTemp;
	
	fTemp = 16383 * fGreen * GREEN_WHITE_BALANCE;
	g_ulColors[GREEN] = (unsigned long) fTemp;
	
	fTemp = 16383 * fBlue * BLUE_WHITE_BALANCE;
	g_ulColors[BLUE] = (unsigned long) fTemp;
	
	RGBColorSet(g_ulColors);
	//Serial.print("a=%d, rgb=(%X, %X, %X)\n", angle,
	//	g_ulColors[RED], g_ulColors[GREEN], g_ulColors[BLUE]);
}

int main(void)
{
	int angle = 0; // angle in degrees

	//Serial.begin(115200);
	RGBInit(0);
	RGBIntensitySet(0.2f);
	RGBEnable();
	
	while(true)
	{
		sinebow(angle);
		delay_ms(10);
		if( ++angle >= 360 ) angle = 0;
	}
}
