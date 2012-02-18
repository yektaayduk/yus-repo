
#ifndef __QUADTASKS_H__
#define __QUADTASKS_H__

#define STEP_DELAY		2	// x10ms

/* tasks priorities */
#define PRIORITY_LEGA_TASK  3
#define PRIORITY_LEGB_TASK  3
#define PRIORITY_LEGC_TASK  3
#define PRIORITY_LEGD_TASK  3

/* tasks stack sizes */
#define SIZE_LEGA_TASK	  200
#define SIZE_LEGB_TASK	  200
#define SIZE_LEGC_TASK	  200
#define SIZE_LEGD_TASK	  200


void taskLegA(void *param);
void taskLegB(void *param);
void taskLegC(void *param);
void taskLegD(void *param);

void CreateQuadTasks(void);

#endif /* __QUADTASKS_H__ */
