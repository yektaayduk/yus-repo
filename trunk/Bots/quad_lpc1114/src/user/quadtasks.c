
#include "LPC11xx.h"
#include "CoOS.h"
#include "quad.h"
#include "quadtasks.h"


OS_STK stkLegA[SIZE_LEGA_TASK];
OS_STK stkLegB[SIZE_LEGB_TASK];
OS_STK stkLegC[SIZE_LEGC_TASK];
OS_STK stkLegD[SIZE_LEGC_TASK];

OS_FlagID gflagElevateA;
OS_FlagID gflagElevateB;
OS_FlagID gflagElevateC;
OS_FlagID gflagElevateD;

void taskLegA(void *param)
{
	uint8_t pos;
	CoTickDelay(5);
	for(;;) {
		CoWaitForSingleFlag( gflagElevateA, 0 );
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			setLeg(LEG_A, AN+pivots[pos], AH+lifts[pos], AS+knees[pos]);
			CoTickDelay(STEP_DELAY);
			if(pos==POSITION_TOTAL>>2)
				CoSetFlag( gflagElevateD );
		}
	}
}

void taskLegB(void *param)
{
	uint8_t pos;
	CoTickDelay(5);
	for(;;) {
		CoWaitForSingleFlag( gflagElevateB, 0 );
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			setLeg(LEG_B, BN+pivots[pos], BH+lifts[pos], BS+knees[pos]);
			CoTickDelay(STEP_DELAY);
			if(pos==POSITION_TOTAL>>2)
				CoSetFlag( gflagElevateA );
		}
	}
}

void taskLegC(void *param)
{
	uint8_t pos;
	CoTickDelay(5);
	for(;;) {
		CoWaitForSingleFlag( gflagElevateC, 0 );
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			setLeg(LEG_C, CN+pivots[POSITION_TOTAL-pos-1], CH+lifts[pos], CS+knees[pos]);
			CoTickDelay(STEP_DELAY);
			if(pos==POSITION_TOTAL>>2)
				CoSetFlag( gflagElevateB );
		}
	}
}

void taskLegD(void *param)
{
	uint8_t pos;
	CoTickDelay(5);
	for(;;) {
		CoWaitForSingleFlag( gflagElevateD, 0 );
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			setLeg(LEG_D, DN+pivots[POSITION_TOTAL-pos-1], DH+lifts[pos], DS+knees[pos]);
			CoTickDelay(STEP_DELAY);
			if(pos==POSITION_TOTAL>>2)
				CoSetFlag( gflagElevateC );
		}
	}
}

void CreateQuadTasks(void)
{
	// initialize servo system
	quad_init();

	// auto reset flags
	gflagElevateA = CoCreateFlag(TRUE, TRUE);
	gflagElevateB = CoCreateFlag(TRUE, FALSE);
	gflagElevateC = CoCreateFlag(TRUE, FALSE);
	gflagElevateD = CoCreateFlag(TRUE, FALSE);

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
}


