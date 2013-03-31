#include <lm4f120xl.h>

void delay()
{
	volatile unsigned long cnt;
	for(cnt = 0; cnt < 0x1FFFFF; cnt++) {}
}

int main(void)
{
	unsigned long colors[3];
	
	RGBInit(1);
	RGBIntensitySet(0.2f);
	
	while(true)
	{
		memset(colors, 0, sizeof(colors));
		RGBColorSet(colors);
		delay();
		colors[0] = 0x8000;
		RGBColorSet(colors);
		delay();
		
		memset(colors, 0, sizeof(colors));
		RGBColorSet(colors);
		delay();
		colors[1] = 0x8000;
		RGBColorSet(colors);
		delay();
		
		memset(colors, 0, sizeof(colors));
		RGBColorSet(colors);
		delay();
		colors[2] = 0x8000;
		RGBColorSet(colors);
		delay();
	}
}
