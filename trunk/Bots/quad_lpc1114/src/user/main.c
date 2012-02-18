
#include "LPC11xx.h"
#include "CoOS.h"
#include "driver_config.h"
#include "target_config.h"

#include "gpio.h"
#include "xuart.h"
#include "servo.h"
#include "quad.h"

#define PRIORITY_BLINK_TASK  2		/* Priority of Blink task */
#define PRIORITY_PRINT_TASK  2		/* Priority of Blink task */
#define PRIORITY_SERVOTEST_TASK  3

#define SIZE_BLINK_TASK	  100    /* Stack size of Blink task */
#define SIZE_PRINT_TASK	  200    /* Stack size of Blink task */
#define SIZE_SERVOTEST_TASK	  200

OS_STK stkBlink[SIZE_BLINK_TASK];	/* Stack of Blink task */
OS_STK stkPrint[SIZE_PRINT_TASK];	/* Stack of Blink task */
OS_STK stkServoTest[SIZE_SERVOTEST_TASK];

void taskBlink(void *param);
void taskPrint(void *param);
void taskServoTest(void *param);

int main(void)
{
	CoInitOS();

	/* Create Blink Task */
	CoCreateTask( taskBlink,
				  (void *)0,
				  PRIORITY_BLINK_TASK,
			      &stkBlink[SIZE_BLINK_TASK-1],
			      SIZE_BLINK_TASK );

#if 0
	/* Create Print Task */
	CoCreateTask( taskPrint,
				  (void *)0,
				  PRIORITY_PRINT_TASK,
				  &stkPrint[SIZE_PRINT_TASK-1],
				  SIZE_PRINT_TASK );
#endif

#if 1
	CoCreateTask( taskServoTest,
				  (void *)0,
				  PRIORITY_SERVOTEST_TASK,
				  &stkPrint[SIZE_SERVOTEST_TASK-1],
				  SIZE_SERVOTEST_TASK );
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
		CoTickDelay(10);	/* Delay 100ms */

		/* Turn Off Led */
		GPIOSetValue( LED_PORT, LED_BIT, LED_OFF );
		CoTickDelay(10);	/* Delay 100ms */
	}
}

void taskPrint(void *param)
{
	static uint32_t cnt = 0;
	xuart_init(115200);

	for(;;) {
		printf("hello, world! (%ld) \n", ++cnt);
		CoTickDelay(50);	/* Delay 500ms */
	}
}

void taskServoTest(void *param)
{
	quad_init();
	uint16_t angle, pwm;

	for(;;) {
		for(angle=30; angle<150; angle++){
			pwm = ANGLE_TO_COUNTER(angle);
			servo_pwm[2] = pwm;
			servo_pwm[5] = pwm;
			servo_pwm[8] = pwm;
			servo_pwm[11] = pwm;
			CoTickDelay(2);
		}
		for(angle=150; angle>30; angle--){
			pwm = ANGLE_TO_COUNTER(angle);
			servo_pwm[2] = pwm;
			servo_pwm[5] = pwm;
			servo_pwm[8] = pwm;
			servo_pwm[11] = pwm;
			CoTickDelay(2);
		}
	}
}

