
#include "LPC11xx.h"
#include "CoOS.h"
#include "driver_config.h"
#include "target_config.h"

#include "gpio.h"
#include "xuart.h"
#include "quadtasks.h"

// from remote joystick
#define DATA_START				0x55
#define MAX_TIMEOUT_COUNT		100 // tick delays
#define MAX_JOYSTICK_RAD		10 // radius value

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
	// joystick data
	uint8_t radiusL, angleL;
	uint8_t radiusR, angleR;
	uint8_t cnt = 0;

	xuart_init(57600);
	g_QuadMovement = CENTER_POS;
	CoTickDelay(200);

	for(;;){
		if( !uart_test() ) {
			// no commands received
			if( ++cnt > MAX_TIMEOUT_COUNT ) {
				g_QuadMovement = CENTER_POS;
				cnt = 0;
			}
			CoTickDelay(1);
			continue;
		}
		if( uart_getc() != DATA_START) continue;
		cnt = 0;
		GPIOSetValue( LED_PORT, LED_BIT, LED_ON );
		radiusL = uart_getc();	angleL = uart_getc();
		radiusR = uart_getc();	angleR = uart_getc();

		if( !radiusL && !angleL && !radiusR && !angleR) continue; // "null" data

		if( radiusR>5 && (angleR>=33 || angleR<3) ) { // "move" command
			if( radiusL ) {
				g_StepDelay = MIN_STEP_DELAY + ((MAX_JOYSTICK_RAD-radiusL)>>1);
				if( angleL>=35 || angleL<5 )
					g_QuadMovement = FORWARD;
				else if( angleL>=5 && angleL<9 )
					g_QuadMovement = LEFTWARD;
				else if( angleL>=9 && angleL<15 )
					g_QuadMovement = LEFT_TURN;
				else if( angleL>=15 && angleL<21 )
					g_QuadMovement = BACKWARD;
				else if( angleL>=21 && angleL<27 )
					g_QuadMovement = RIGHT_TURN;
				else if( angleL>=27 && angleL<35 )
					g_QuadMovement = RIGHTWARD;
			}
			else g_QuadMovement = CENTER_POS;
		}
		else {
			g_InclineAngle = (angleL<<3) + (angleL<<1); // angle x 10
			g_QuadMovement = INCLINED_POS;
		}
		CoTickDelay(1);
	}
}

