
#include "LPC11xx.h"
#include "CoOS.h"
#include "driver_config.h"
#include "target_config.h"

#include "gpio.h"
#include "xuart.h"
#include "quadtasks.h"

/* led blinker task */
#define PRIORITY_BLINK_TASK			5
#define SIZE_BLINK_TASK				50
OS_STK stkBlink[SIZE_BLINK_TASK];
void taskBlink(void *param);

/* user task */
#define PRIORITY_USER_TASK			4
#define SIZE_USER_TASK				200
OS_STK stkUser[SIZE_USER_TASK];
void taskUser(void *param);


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

	/* Create User Task */
	CoCreateTask( taskUser,
				  (void *)0,
				  PRIORITY_USER_TASK,
			      &stkUser[SIZE_USER_TASK-1],
			      SIZE_USER_TASK );

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

void taskUser(void *param)
{
	for(;;){
		g_QuadDirection = STEADY;
		CoTickDelay(200);
		g_QuadDirection = FORWARD;
		CoTickDelay(700);
		//g_QuadDirection = BACKWARD;
		//CoTickDelay(700);
		//g_QuadDirection = STEADY;
		//CoTickDelay(200);
		g_QuadDirection = LEFT_TURN;
		CoTickDelay(700);
		g_QuadDirection = RIGHT_TURN;
		CoTickDelay(700);
	}
}

