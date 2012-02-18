
#include "LPC11xx.h"
#include "CoOS.h"
#include "driver_config.h"
#include "target_config.h"

#include "gpio.h"
#include "xuart.h"
#include "quadtasks.h"

/* led blinker task */
#define PRIORITY_BLINK_TASK			7
#define SIZE_BLINK_TASK				50
OS_STK stkBlink[SIZE_BLINK_TASK];
void taskBlink(void *param);

/* user task */
#define PRIORITY_USER_TASK			6
#define SIZE_USER_TASK				200
OS_STK stkUser[SIZE_USER_TASK];
void taskUser(void *param);


int main(void)
{
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
		CoTickDelay(10);	/* Delay 100ms */

		/* Turn Off Led */
		GPIOSetValue( LED_PORT, LED_BIT, LED_OFF );
		CoTickDelay(90);	/* Delay 900ms */
	}
}

void taskUser(void *param)
{
	uint8_t cmd;

	xuart_init(57600);
	g_QuadDirection = STEADY;
	CoTickDelay(200);

	for(;;){
		if( !uart_test() ){
			CoTickDelay(10);
			continue;
		}
		cmd = uart_getc();
		switch(cmd)
		{
		case 'w':
		case 'W':
			g_QuadDirection = FORWARD;
			break;
		case 'a':
		case 'A':
			g_QuadDirection = LEFT_TURN;
			break;
		case 's':
		case 'S':
			g_QuadDirection = BACKWARD;
			break;
		case 'd':
		case 'D':
			g_QuadDirection = RIGHT_TURN;
			break;
		case 'q':
		case 'Q':
			g_QuadDirection = LEFTWARD;
			break;
		case 'e':
		case 'E':
			g_QuadDirection = RIGHTWARD;
			break;
		case 'x':
		case 'X':
		default:
			g_QuadDirection = STEADY;
			break;
		}
	}
}

