#include <lm4f120xl.h>

void delay()
{
	volatile unsigned long cnt;
	for(cnt = 0; cnt < 0xFFFFF; cnt++) {}
}

int main(void)
{
	unsigned long colors[3];
	
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);
	
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
