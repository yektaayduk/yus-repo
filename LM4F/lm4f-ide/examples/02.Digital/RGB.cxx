#include <lm4f120xl.h>

int main(void)
{
	unsigned long colors[3];
	
	RGBInit(1);
	RGBIntensitySet(0.05f);
	
	while(true)
	{
		memset(colors, 0, sizeof(colors));
		RGBColorSet(colors);
		delay_ms(500);
		colors[0] = 0x8000;
		RGBColorSet(colors);
		delay_ms(500);
		
		memset(colors, 0, sizeof(colors));
		RGBColorSet(colors);
		delay_ms(500);
		colors[1] = 0x8000;
		RGBColorSet(colors);
		delay_ms(500);
		
		memset(colors, 0, sizeof(colors));
		RGBColorSet(colors);
		delay_ms(500);
		colors[2] = 0x8000;
		RGBColorSet(colors);
		delay_ms(500);
	}
}
