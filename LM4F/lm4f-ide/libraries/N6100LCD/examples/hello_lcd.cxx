#include <lm4f120xl.h>
#include <N6100LCD.h>

// N6100LCD(DAT_PIN, CLK_PIN, CS_PIN, RST_PIN)
N6100LCD mylcd(PB4, PA5, PA6, PA7);

int main(void)
{
	mylcd.init();
	//mylcd.clear(LCD_WHITE);
	mylcd.clear();
	//mylcd.text("hello world!", 20, 70, LCD_RED, LCD_WHITE);
	mylcd.text("hello world!", 20, 50);
	
	while(true)	{ }
}
