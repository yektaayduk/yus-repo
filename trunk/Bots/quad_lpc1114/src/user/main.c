
#include "LPC11xx.h"
#include "CoOS.h"
#include "driver_config.h"
#include "target_config.h"

#include "gpio.h"
#include "xuart.h"
#include "quad.h"

#define PRIORITY_BLINK_TASK  2		/* Priority of Blink task */
#define PRIORITY_LEGA_TASK  3
#define PRIORITY_LEGB_TASK  3

#define SIZE_BLINK_TASK	  100    /* Stack size of Blink task */
#define SIZE_LEGA_TASK	  200
#define SIZE_LEGB_TASK	  200

OS_STK stkBlink[SIZE_BLINK_TASK];	/* Stack of Blink task */
OS_STK stkLegA[SIZE_LEGA_TASK];
OS_STK stkLegB[SIZE_LEGB_TASK];

void taskBlink(void *param);
void taskLegA(void *param);
void taskLegB(void *param);

int main(void)
{
	quad_init();
	xuart_init(115200);

	CoInitOS();

	/* Create Blink Task */
	CoCreateTask( taskBlink,
				  (void *)0,
				  PRIORITY_BLINK_TASK,
			      &stkBlink[SIZE_BLINK_TASK-1],
			      SIZE_BLINK_TASK );

#if 1
	CoCreateTask( taskLegA,
				  (void *)0,
				  PRIORITY_LEGA_TASK,
				  &stkLegA[SIZE_LEGA_TASK-1],
				  SIZE_LEGA_TASK );
#endif

#if 1
	CoCreateTask( taskLegB,
				  (void *)0,
				  PRIORITY_LEGB_TASK,
				  &stkLegB[SIZE_LEGB_TASK-1],
				  SIZE_LEGB_TASK );
#endif

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

void taskLegA(void *param)
{
	CoTickDelay(200);
	for(;;) {
		setLeg(LEG_A, AN, AF, AS-20);
		CoTickDelay(100);
		setLeg(LEG_A, AN+35, AF, AS-30);
		CoTickDelay(100);
		setLeg(LEG_A, AN, AF+30, AS+40);
		CoTickDelay(100);
		setLeg(LEG_A, AN-35, AF, AS-30);
		CoTickDelay(100);
	}
}

void taskLegB(void *param)
{
	CoTickDelay(100);
	for(;;) {
		setLeg(LEG_B, BN+35, BF, BS-30);
		CoTickDelay(100);
		setLeg(LEG_B, BN, BF+30, BS+40);
		CoTickDelay(100);
		setLeg(LEG_B, BN-35, BF, BS-30);
		CoTickDelay(100);
		setLeg(LEG_B, BN, BF, BS-20);
		CoTickDelay(100);
	}
}

