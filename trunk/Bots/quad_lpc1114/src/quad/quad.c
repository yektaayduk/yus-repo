
#include "LPC11xx.h"
#include "quad.h"

extern const uint16_t pwm_values[180];

void quad_init(void)
{
	servos_init();
}

void setLeg(leg_t leg, uint8_t pivot, uint8_t lift, uint8_t knee)
{
	switch(leg)
	{
	case LEG_A:
		servo_pwm[1-1] = pwm_values[179-pivot];
		servo_pwm[2-1] = pwm_values[lift];
		servo_pwm[3-1] = pwm_values[179-knee];
		break;
	case LEG_B:
		servo_pwm[4-1] = pwm_values[179-pivot];
		servo_pwm[5-1] = pwm_values[179-lift];
		servo_pwm[6-1] = pwm_values[knee];
		break;
	case LEG_C:
		servo_pwm[7-1] = pwm_values[179-pivot];
		servo_pwm[8-1] = pwm_values[179-lift];
		servo_pwm[9-1] = pwm_values[knee];
		break;
	case LEG_D:
		servo_pwm[10-1] = pwm_values[179-pivot];
		servo_pwm[11-1] = pwm_values[lift];
		servo_pwm[12-1] = pwm_values[179-knee];
		break;
	}
}

#if 0
const int8_t pivots[32] =
{
	0,		-8,		-15,	-22,	-28,	-33,	-37,	-39,
	-40,	-39,	-37,	-33,	-28,	-22,	-15,	-8,
	0,		8,		15,		22,		28,		33,		37,		39,
	40,		39,		37,		33,		28,		22,		15,		8,
};

const int8_t lifts[32] =
{
	40,		36,		32,		28,		25,		21,		18,		15,
	12,		9,		7,		5,		3,		2,		1,		0,
	0,		0,		1,		2,		3,		5,		7,		9,
	12,		15,		18,		21,		25,		28,		32,		36,
};

const int8_t knees[32] =
{
	30,		15,		1,		-12,	-24,	-35,	-38,	-35,
	-32,	-29,	-27,	-25,	-23,	-22,	-21,	-20,
	-20,	-20,	-21,	-22,	-23,	-25,	-27,	-29,
	-32,	-35,	-38,	-35,	-24,	-12,	1,		15,
};
#else
const int8_t pivots[POSITION_TOTAL] =
{
	0,		-6,		-11,	-16,	-21,	-26,	-30,	-33,
	-36,	-38,	-39,	-40,	-40,	-39,	-38,	-37,
	-36,	-35,	-33,	-32,	-30,	-28,	-26,	-24,
	-21,	-19,	-16,	-14,	-11,	-8,		-6,		-3,
	0,		3,		6,		8,		11,		14,		16,		19,
	21,		24,		26,		28,		30,		32,		33,		35,
	36,		37,		38,		39,		40,		40,		39,		38,
	36,		33,		30,		26,		21,		16,		11,		6,
};

const int8_t lifts[POSITION_TOTAL] =
{
	50,		48,		46,		44,		42,		40,		38,		37,
	35,		33,		31,		29,		28,		26,		25,		23,
	22,		20,		19,		18,		17,		16,		15,		14,
	13,		12,		12,		11,		11,		10,		10,		10,
	10,		10,		10,		10,		11,		11,		12,		12,
	13,		14,		15,		16,		17,		18,		19,		20,
	22,		23,		25,		26,		28,		29,		31,		33,
	35,		37,		38,		40,		42,		44,		46,		48,
};

const int8_t knees[POSITION_TOTAL] =
{
	28,		19,		11,		3,		-5,		-12,	-19,	-25,
	-30,	-33,	-33,	-32,	-31,	-29,	-28,	-26,
	-25,	-24,	-23,	-22,	-21,	-20,	-19,	-18,
	-18,	-17,	-17,	-16,	-16,	-15,	-15,	-15,
	-15,	-15,	-15,	-15,	-16,	-16,	-17,	-17,
	-18,	-18,	-19,	-20,	-21,	-22,	-23,	-24,
	-25,	-26,	-28,	-29,	-31,	-32,	-33,	-33,
	-30,	-25,	-19,	-12,	-5,		3,		11,		19,
};


#endif
