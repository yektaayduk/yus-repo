
#ifndef __QUADTASKS_H__
#define __QUADTASKS_H__

#include "quad.h"

#define STEP_DELAY		2	// x10ms

// interval of A->D & C->B
//#define LEG_INTERVAL_1	(POSITION_TOTAL/8)
#define LEG_INTERVAL_1	(POSITION_TOTAL/10)
// interval of D->C & B->A
#define LEG_INTERVAL_2	(POSITION_TOTAL/4)
//#define LEG_FINAL_POS	(POSITION_TOTAL*3/4) // 75%
#define LEG_FINAL_POS	(POSITION_TOTAL-LEG_INTERVAL_1)

/* tasks priorities */
#define PRIORITY_LEGA_TASK  5
#define PRIORITY_LEGB_TASK  5
#define PRIORITY_LEGC_TASK  5
#define PRIORITY_LEGD_TASK  5

/* tasks stack sizes */
#define SIZE_LEGA_TASK	  200
#define SIZE_LEGB_TASK	  200
#define SIZE_LEGC_TASK	  200
#define SIZE_LEGD_TASK	  200

typedef enum{
	STEADY = 0,
	FORWARD,
	BACKWARD,
	LEFTWARD,
	RIGHTWARD,
	LEFT_TURN,
	RIGHT_TURN
}direction_t;

extern volatile direction_t g_QuadDirection;

void taskLegA(void *param);
void taskLegB(void *param);
void taskLegC(void *param);
void taskLegD(void *param);

void CreateQuadTasks(void);

#endif /* __QUADTASKS_H__ */
