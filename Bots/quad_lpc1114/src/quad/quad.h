

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
#define AN	85
#define BN	95
#define CN	90
#define DN	95

// lift flats
#define AF	85
#define BF	90
#define CF	90
#define DF	90

// knee straight
#define AS	120
#define BS	120
#define CS	110
#define DS	120

#endif /* __QUAD_H__ */
