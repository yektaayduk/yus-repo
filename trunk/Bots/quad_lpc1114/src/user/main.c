
#include "LPC11xx.h"
#include "CoOS.h"
#include "driver_config.h"
#include "target_config.h"

#include "gpio.h"
#include "xuart.h"
#include "quadtasks.h"

/* led blinker tasks */
#define PRIORITY_BLINK_TASK  2	/* Priority of Blink task */
#define SIZE_BLINK_TASK	  100	/* Stack size of Blink task */
OS_STK stkBlink[SIZE_BLINK_TASK];	/* Stack of Blink task */
void taskBlink(void *param);


int main(void)
{
	xuart_init(115200);

	CoInitOS();

	/* Create Blink Task */
	CoCreateTask( taskBlink,
				  (void *)0,
				  PRIORITY_BLINK_TASK,
			      &stkBlink[SIZE_BLINK_TASK-1],
			      SIZE_BLINK_TASK );

	/* Quadruped tasks*/
	CreateQuadTasks();

	CoStartOS();

	while(1);
}

void taskBlink(void *param)
{
	/* Enable GPIO block clock */
	GPIOInit();

	/* Set LED port pin to output */
	GPIOSetDir( LED_PORT, LED_BIT, 1 );

	for(;;) {
		/* Turn On Led */
		GPIOSetValue( LED_PORT, LED_BIT, LED_ON );
		CoTickDelay(50);	/* Delay 500ms */

		/* Turn Off Led */
		GPIOSetValue( LED_PORT, LED_BIT, LED_OFF );
		CoTickDelay(50);	/* Delay 500ms */
	}
}


