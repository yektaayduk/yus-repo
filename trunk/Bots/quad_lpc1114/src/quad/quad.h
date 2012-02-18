

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
inline void setLeg(leg_t leg, uint8_t pivot, uint8_t lift, uint8_t knee);

#endif /* __QUAD_H__ */
