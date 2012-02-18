
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

const int8_t pivots[POSITION_TOTAL] =
{
	0,		-6,		-11,	-16,	-21,	-26,	-30,	-33,
	-36,	-38,	-39,	-40,	-40,	-39,	-38,	-37,
	-36,	-35,	-33,	-32,	-30,	-28,	-26,	-24,
	-21,	-19,	-16,	-14,	-11,	-8,		-6,		-3,
	0,		3,		6,		8,		11,		14,		16,		19,
	21,		24,		26,		28,		30,		32,		33,		35,
	36,		37,		38,		39,		40,		40,		39,		38,
	36,		33,		30,		26,		21,		16,		11,		6
};

const int8_t lifts[POSITION_TOTAL] =
{
	56,		50,		45,		40,		35,		30,		25,		20,
	16,		15,		15,		17,		19,		21,		23,		25,
	26,		28,		29,		31,		32,		33,		34,		35,
	36,		37,		37,		38,		38,		39,		39,		39,
	39,		39,		39,		39,		38,		38,		37,		37,
	36,		35,		34,		33,		32,		31,		29,		28,
	26,		25,		23,		21,		19,		17,		15,		15,
	16,		20,		25,		30,		35,		40,		45,		50
};

const int8_t knees[POSITION_TOTAL] =
{
	31,		21,		11,		2,		-8,		-16,	-25,	-33,
	-41,	-43,	-46,	-47,	-49,	-51,	-52,	-53,
	-55,	-56,	-57,	-58,	-59,	-60,	-61,	-62,
	-62,	-63,	-64,	-64,	-64,	-65,	-65,	-65,
	-65,	-65,	-65,	-65,	-64,	-64,	-64,	-63,
	-62,	-62,	-61,	-60,	-59,	-58,	-57,	-56,
	-55,	-53,	-52,	-51,	-49,	-47,	-46,	-43,
	-41,	-33,	-25,	-16,	-8,		2,		11,		21
};


