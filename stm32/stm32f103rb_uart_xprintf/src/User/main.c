/********************************************
 * project: STM32F10X xprintf UART
 *    file: main.c
 ********************************************/

#include "stm32f10x.h"
#include "xuart.h"

int main()
{
	char buff[50];

	xuart_init(115200);

	xprintf("hello stm%d!\r\n", 32);

	while(1)
	{
		xprintf("what's your name?: ");
		xgets(buff, 50);
		if(buff[0])
			xprintf("hi there, %s!\n", buff);
	}
	return 0;
}

