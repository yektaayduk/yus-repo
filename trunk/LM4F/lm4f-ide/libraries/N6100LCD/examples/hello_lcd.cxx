#include <lm4f120xl.h>
#include <N6100LCD.h>

// N6100LCD(DAT_PIN, CLK_PIN, CS_PIN, RST_PIN)
N6100LCD mylcd(PB4, PA5, PA6, PA7);

int main(void)
{
	mylcd.init();
	mylcd.text("hello world!", 10, 20, LCD_RED, LCD_WHITE);
	
	while(true)	{ }
}
