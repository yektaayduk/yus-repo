
#include "LPC11xx.h"
#include "CoOS.h"
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
	CoWaitForSingleFlag( gflagElevateA, 0 );
	for(;;) {
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			if(pos==LEG_INTERVAL_1)
				CoSetFlag( gflagElevateD );
			setLegA( AN+pivots[pos], AH+lifts[pos], AS+knees[pos] );
			CoTickDelay(STEP_DELAY);
			if(pos==LEG_FINAL_POS)
				CoWaitForSingleFlag( gflagElevateA, 0 );
		}
	}
}

void taskLegB(void *param)
{
	uint8_t pos;
	CoTickDelay(5);
	CoWaitForSingleFlag( gflagElevateB, 0 );
	for(;;) {
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			if(pos==LEG_INTERVAL_2)
				CoSetFlag( gflagElevateA );
			setLegB( BN+pivots[pos], BH+lifts[pos], BS+knees[pos] );
			CoTickDelay(STEP_DELAY);
			if(pos==LEG_FINAL_POS)
				CoWaitForSingleFlag( gflagElevateB, 0 );
		}
	}
}

void taskLegC(void *param)
{
	uint8_t pos;
	CoTickDelay(5);
	CoWaitForSingleFlag( gflagElevateC, 0 );
	for(;;) {
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			if(pos==LEG_INTERVAL_1)
				CoSetFlag( gflagElevateB );
			setLegC( CN+pivots[POSITION_TOTAL-pos-1], CH+lifts[pos], CS+knees[pos] );
			CoTickDelay(STEP_DELAY);
			if(pos==LEG_FINAL_POS)
				CoWaitForSingleFlag( gflagElevateC, 0 );
		}
	}
}

void taskLegD(void *param)
{
	uint8_t pos;
	CoTickDelay(5);
	CoWaitForSingleFlag( gflagElevateD, 0 );
	for(;;) {
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			if(pos==LEG_INTERVAL_2)
				CoSetFlag( gflagElevateC );
			setLegD( DN+pivots[POSITION_TOTAL-pos-1], DH+lifts[pos], DS+knees[pos] );
			CoTickDelay(STEP_DELAY);
			if(pos==LEG_FINAL_POS)
				CoWaitForSingleFlag( gflagElevateD, 0 );
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

	CoCreateTask( taskLegA,
				  (void *)0,
				  PRIORITY_LEGA_TASK,
				  &stkLegA[SIZE_LEGA_TASK-1],
				  SIZE_LEGA_TASK );

	CoCreateTask( taskLegB,
				  (void *)0,
				  PRIORITY_LEGB_TASK,
				  &stkLegB[SIZE_LEGB_TASK-1],
				  SIZE_LEGB_TASK );

	CoCreateTask( taskLegC,
				  (void *)0,
				  PRIORITY_LEGC_TASK,
				  &stkLegC[SIZE_LEGC_TASK-1],
				  SIZE_LEGC_TASK );

	CoCreateTask( taskLegD,
				  (void *)0,
				  PRIORITY_LEGD_TASK,
				  &stkLegD[SIZE_LEGD_TASK-1],
				  SIZE_LEGD_TASK );

}


