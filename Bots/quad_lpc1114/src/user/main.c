
#include "LPC11xx.h"
#include "CoOS.h"
#include "driver_config.h"
#include "target_config.h"

#include "gpio.h"
#include "xuart.h"
#include "quad.h"

#define STEP_DELAY		2	// x10ms

#define PRIORITY_BLINK_TASK  2		/* Priority of Blink task */
#define PRIORITY_LEGA_TASK  3
#define PRIORITY_LEGB_TASK  3
#define PRIORITY_LEGC_TASK  3
#define PRIORITY_LEGD_TASK  3

#define SIZE_BLINK_TASK	  100    /* Stack size of Blink task */
#define SIZE_LEGA_TASK	  200
#define SIZE_LEGB_TASK	  200
#define SIZE_LEGC_TASK	  200
#define SIZE_LEGD_TASK	  200

OS_STK stkBlink[SIZE_BLINK_TASK];	/* Stack of Blink task */
OS_STK stkLegA[SIZE_LEGA_TASK];
OS_STK stkLegB[SIZE_LEGB_TASK];
OS_STK stkLegC[SIZE_LEGC_TASK];
OS_STK stkLegD[SIZE_LEGC_TASK];

void taskBlink(void *param);
void taskLegA(void *param);
void taskLegB(void *param);
void taskLegC(void *param);
void taskLegD(void *param);

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

#if 1
	CoCreateTask( taskLegC,
				  (void *)0,
				  PRIORITY_LEGC_TASK,
				  &stkLegC[SIZE_LEGC_TASK-1],
				  SIZE_LEGC_TASK );
#endif

#if 1
	CoCreateTask( taskLegD,
				  (void *)0,
				  PRIORITY_LEGD_TASK,
				  &stkLegD[SIZE_LEGD_TASK-1],
				  SIZE_LEGD_TASK );
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
	uint8_t pos;
	//CoTickDelay(0);
	for(;;) {
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			setLeg(LEG_A, AN+pivots[pos], AH+lifts[pos], AS+knees[pos]);
			CoTickDelay(STEP_DELAY);
		}
	}
}

void taskLegB(void *param)
{
	uint8_t pos;
	CoTickDelay(STEP_DELAY*POSITION_TOTAL*3/4);
	for(;;) {
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			setLeg(LEG_B, BN+pivots[pos], BH+lifts[pos], BS+knees[pos]);
			CoTickDelay(STEP_DELAY);
		}
	}
}

void taskLegC(void *param)
{
	uint8_t pos;
	CoTickDelay(STEP_DELAY*POSITION_TOTAL*2/4);
	for(;;) {
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			setLeg(LEG_C, CN+pivots[POSITION_TOTAL-pos-1], CH+lifts[pos], CS+knees[pos]);
			CoTickDelay(STEP_DELAY);
		}
	}
}

void taskLegD(void *param)
{
	uint8_t pos;
	CoTickDelay(STEP_DELAY*POSITION_TOTAL*1/4);
	for(;;) {
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			setLeg(LEG_D, DN+pivots[POSITION_TOTAL-pos-1], DH+lifts[pos], DS+knees[pos]);
			CoTickDelay(STEP_DELAY);
		}
	}
}
