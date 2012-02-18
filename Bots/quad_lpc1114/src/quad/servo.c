
#include "LPC11xx.h"
#include "CoOS.h"
#include "servo.h"

volatile uint16_t servo_pwm[NSERVOS];
volatile uint16_t _pwm_cnt;

void timer32_0_init(void)
{
	/* enable AHB clock */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);

	LPC_TMR32B0->MR0 = (uint32_t)(SystemCoreClock/SERVO_INTERRUPT_RATE);

	/* Interrupt and Reset on MR0 */
	LPC_TMR32B0->MCR = 3;

	/* Enable the TIMER0 Interrupt */
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
	/* enable timer 0 */
	LPC_TMR32B0->TCR = 1;
}

void servos_init(void)
{
	uint8_t i;

	/* Enable AHB clock to the GPIO domain. */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);

	/* Set servo pins to output */
	// servo  1 : PIO1_5  */
	SERVO1_PORT->DIR |= SERVO1_PIN;

	// servo  2 : PIO1_8
	SERVO2_PORT->DIR |= SERVO2_PIN;

	// servo  3 : PIO0_6
	SERVO3_PORT->DIR |= SERVO3_PIN;

	// servo  4 : PIO3_0
	SERVO4_PORT->DIR |= SERVO4_PIN;

	// servo  5 : PIO3_1
	SERVO5_PORT->DIR |= SERVO5_PIN;

	// servo  6 : PIO3_2
	SERVO6_PORT->DIR |= SERVO6_PIN;

	// servo  7 : PIO2_6
	SERVO7_PORT->DIR |= SERVO7_PIN;

	// servo  8 : PIO2_7
	SERVO8_PORT->DIR |= SERVO8_PIN;

	// servo  9 : PIO2_8
	SERVO9_PORT->DIR |= SERVO9_PIN;

	// servo 10 : PIO2_9
	SERVO10_PORT->DIR |= SERVO10_PIN;

	// servo 11 : PIO2_10
	SERVO11_PORT->DIR |= SERVO11_PIN;

	// servo 12 : PIO3_3
	SERVO12_PORT->DIR |= SERVO12_PIN;

	for(i=0; i<NSERVOS; i++)
	{
		servo_pwm[i] = INITIAL_COUNT;
	}

	// software PWM
	_pwm_cnt = 0;
	timer32_0_init();
}

void TIMER32_0_IRQHandler(void)
{
	//CoEnterISR();
	if ( LPC_TMR32B0->IR & 0x01 )
	{
		// max duty reached
		if( _pwm_cnt++ > SERVO_MAX_COUNT ){
			SERVO1_LOW();  SERVO2_LOW();  SERVO3_LOW();
			SERVO4_LOW();  SERVO5_LOW();  SERVO6_LOW();
			SERVO7_LOW();  SERVO8_LOW();  SERVO9_LOW();
			SERVO10_LOW(); SERVO11_LOW(); SERVO12_LOW();
		}
		else if( _pwm_cnt > SERVO_MIN_COUNT ){
			if( _pwm_cnt > servo_pwm[0]) SERVO1_LOW();
			if( _pwm_cnt > servo_pwm[1]) SERVO2_LOW();
			if( _pwm_cnt > servo_pwm[2]) SERVO3_LOW();
			if( _pwm_cnt > servo_pwm[3]) SERVO4_LOW();
			if( _pwm_cnt > servo_pwm[4]) SERVO5_LOW();
			if( _pwm_cnt > servo_pwm[5]) SERVO6_LOW();
			if( _pwm_cnt > servo_pwm[6]) SERVO7_LOW();
			if( _pwm_cnt > servo_pwm[7]) SERVO8_LOW();
			if( _pwm_cnt > servo_pwm[8]) SERVO9_LOW();
			if( _pwm_cnt > servo_pwm[9]) SERVO10_LOW();
			if( _pwm_cnt > servo_pwm[10]) SERVO11_LOW();
			if( _pwm_cnt > servo_pwm[11]) SERVO12_LOW();
		}
		if( _pwm_cnt > SERVO_RESET_COUNT ){
			_pwm_cnt = 0;
			SERVO1_HIGH();  SERVO2_HIGH();  SERVO3_HIGH();
			SERVO4_HIGH();  SERVO5_HIGH();  SERVO6_HIGH();
			SERVO7_HIGH();  SERVO8_HIGH();  SERVO9_HIGH();
			SERVO10_HIGH(); SERVO11_HIGH(); SERVO12_HIGH();
		}

		LPC_TMR32B0->IR = 1;				/* clear interrupt flag */
	}
	//CoExitISR();
}

