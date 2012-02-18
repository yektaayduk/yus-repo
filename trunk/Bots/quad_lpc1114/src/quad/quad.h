

#ifndef __QUAD_H__
#define __QUAD_H__

#include "servo.h"

/****************************
 *     \             /
 *    3 \ A      C  / 9
 *     2 \         / 8
 *      1 |-------| 7
 *        |       |
 *        |       |
 *      4 |-------| 10
 *     5 /         \ 11
 *    6 / B      D  \ 12
 *     /             \
 ****************************/

// quad legs
typedef enum{
	LEG_A = 0,
	LEG_B,
	LEG_C,
	LEG_D
}leg_t;


void quad_init(void);
void setLeg(leg_t leg, uint8_t pivot, uint8_t lift, uint8_t knee);

/* leg assembly compensation */
// pivot normals to CoG
#define AN	95
#define BN	95
#define CN	90
#define DN	105

// lift horizontal
#define AH	85
#define BH	90
#define CH	90
#define DH	90

// knee straight
#define AS	120
#define BS	110
#define CS	120
#define DS	120

/* steps table */

#define POSITION_TOTAL	64

extern const int8_t pivots[POSITION_TOTAL];
extern const int8_t lifts[POSITION_TOTAL];
extern const int8_t knees[POSITION_TOTAL];


#endif /* __QUAD_H__ */
