
#include "LPC11xx.h"
#include "quad.h"

void quad_init(void)
{
	servos_init();
}

inline void setLeg(leg_t leg, uint8_t pivot, uint8_t lift, uint8_t knee)
{
	switch(leg)
	{
	case LEG_A:
		servo_pwm[1-1] = ANGLE_TO_COUNTER(180-pivot);
		servo_pwm[2-1] = ANGLE_TO_COUNTER(lift);
		servo_pwm[3-1] = ANGLE_TO_COUNTER(180-knee);
		break;
	case LEG_B:
		servo_pwm[4-1] = ANGLE_TO_COUNTER(188-pivot);
		servo_pwm[5-1] = ANGLE_TO_COUNTER(180-lift);
		servo_pwm[6-1] = ANGLE_TO_COUNTER(knee);
		break;
	case LEG_C:
		servo_pwm[7-1] = ANGLE_TO_COUNTER(180-pivot);
		servo_pwm[8-1] = ANGLE_TO_COUNTER(180-lift);
		servo_pwm[9-1] = ANGLE_TO_COUNTER(knee);
		break;
	case LEG_D:
		servo_pwm[10-1] = ANGLE_TO_COUNTER(180-pivot);
		servo_pwm[11-1] = ANGLE_TO_COUNTER(lift);
		servo_pwm[12-1] = ANGLE_TO_COUNTER(180-knee);
		break;
	}
}
