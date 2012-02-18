
#include "LPC11xx.h"
#include "CoOS.h"
#include "quadtasks.h"


OS_STK stkLegA[SIZE_LEGA_TASK];
OS_STK stkLegB[SIZE_LEGB_TASK];
OS_STK stkLegC[SIZE_LEGC_TASK];
OS_STK stkLegD[SIZE_LEGC_TASK];

// sequence flags
OS_FlagID g_flagNextA;
OS_FlagID g_flagNextB;
OS_FlagID g_flagNextC;
OS_FlagID g_flagNextD;

volatile direction_t g_QuadDirection;

void taskLegA(void *param)
{
	uint8_t pos;
	CoTickDelay(5);
	CoWaitForSingleFlag( g_flagNextA, 0 );
	for(;;) {
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			if(pos==LEG_INTERVAL_1)
				CoSetFlag( g_flagNextD );

			switch(g_QuadDirection){
			case FORWARD:
			case RIGHTWARD:
			case RIGHT_TURN:
				setLegA( A_FW+pivots[pos], AH+lifts[pos], AS+knees[pos] );
				break;
			case BACKWARD:
			case LEFTWARD:
			case LEFT_TURN:
				setLegA( A_BW+pivots[POSITION_TOTAL-pos-1], AH+lifts[pos], AS+knees[pos] );
				break;
			case CENTER:
				setLegA( AN+pivots[POSITION_TOTAL>>1], AH+lifts[POSITION_TOTAL>>1], AS+knees[POSITION_TOTAL>>1] );
				break;
			default:
				break;
			}

			CoTickDelay(STEP_DELAY);
			if(pos==LEG_FINAL_POS)
				CoWaitForSingleFlag( g_flagNextA, 0 );
		}
	}
}

void taskLegB(void *param)
{
	uint8_t pos;
	CoTickDelay(5);
	CoWaitForSingleFlag( g_flagNextB, 0 );
	for(;;) {
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			if(pos==LEG_INTERVAL_2)
				CoSetFlag( g_flagNextA );

			switch(g_QuadDirection){
			case FORWARD:
			case LEFTWARD:
			case RIGHT_TURN:
				setLegB( B_FW+pivots[pos], BH+lifts[pos], BS+knees[pos] );
				break;
			case BACKWARD:
			case RIGHTWARD:
			case LEFT_TURN:
				setLegB( B_BW+pivots[POSITION_TOTAL-pos-1], BH+lifts[pos], BS+knees[pos] );
				break;
			case CENTER:
				setLegB( BN+pivots[POSITION_TOTAL>>1], BH+lifts[POSITION_TOTAL>>1], BS+knees[POSITION_TOTAL>>1] );
				break;
			default:
				break;
			}

			CoTickDelay(STEP_DELAY);
			if(pos==LEG_FINAL_POS)
				CoWaitForSingleFlag( g_flagNextB, 0 );
		}
	}
}

void taskLegC(void *param)
{
	uint8_t pos;
	CoTickDelay(5);
	CoWaitForSingleFlag( g_flagNextC, 0 );
	for(;;) {
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			if(pos==LEG_INTERVAL_1)
				CoSetFlag( g_flagNextB );

			switch(g_QuadDirection){
			case FORWARD:
			case LEFTWARD:
			case LEFT_TURN:
				setLegC( C_FW+pivots[POSITION_TOTAL-pos-1], CH+lifts[pos], CS+knees[pos] );
				break;
			case BACKWARD:
			case RIGHTWARD:
			case RIGHT_TURN:
				setLegC( C_BW+pivots[pos], CH+lifts[pos], CS+knees[pos] );
				break;
			case CENTER:
				setLegC( CN+pivots[POSITION_TOTAL>>1], CH+lifts[POSITION_TOTAL>>1], CS+knees[POSITION_TOTAL>>1] );
				break;
			default:
				break;
			}

			CoTickDelay(STEP_DELAY);
			if(pos==LEG_FINAL_POS)
				CoWaitForSingleFlag( g_flagNextC, 0 );
		}
	}
}

void taskLegD(void *param)
{
	uint8_t pos;
	CoTickDelay(5);
	CoWaitForSingleFlag( g_flagNextD, 0 );
	for(;;) {
		for(pos=0; pos<POSITION_TOTAL; pos++)
		{
			if(pos==LEG_INTERVAL_2)
				CoSetFlag( g_flagNextC );

			switch(g_QuadDirection){
			case FORWARD:
			case RIGHTWARD:
			case LEFT_TURN:
				setLegD( D_FW+pivots[POSITION_TOTAL-pos-1], DH+lifts[pos], DS+knees[pos] );
				break;
			case BACKWARD:
			case LEFTWARD:
			case RIGHT_TURN:
				setLegD( D_BW+pivots[pos], DH+lifts[pos], DS+knees[pos] );
				break;
			case CENTER:
				setLegD( DN+pivots[POSITION_TOTAL>>1], DH+lifts[POSITION_TOTAL>>1], DS+knees[POSITION_TOTAL>>1] );
				break;
			default:
				break;
			}

			CoTickDelay(STEP_DELAY);
			if(pos==LEG_FINAL_POS)
				CoWaitForSingleFlag( g_flagNextD, 0 );
		}
	}
}

void CreateQuadTasks(void)
{
	// initialize servo system
	quad_init();

	// auto reset flags
	g_flagNextA = CoCreateFlag(TRUE, TRUE);
	g_flagNextB = CoCreateFlag(TRUE, FALSE);
	g_flagNextC = CoCreateFlag(TRUE, FALSE);
	g_flagNextD = CoCreateFlag(TRUE, FALSE);

	g_QuadDirection = CENTER; // REMAIN;

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


