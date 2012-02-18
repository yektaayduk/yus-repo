
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
