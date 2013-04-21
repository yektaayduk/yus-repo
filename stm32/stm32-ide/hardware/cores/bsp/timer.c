
#include <stm32f10x.h>
#include <timer.h>

void delaytimer_init(void) 
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	/* Enable timer clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15 | RCC_APB2Periph_TIM17, ENABLE );

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = UINT16_MAX; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	// TIM15 - 1MHz
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000) - 1;
	TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);
  #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || (defined STM32F10X_HD_VL)
	// TIM17 - 1kHz @ 24MHz clock
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000) - 1;
  #else
	// TIM17 - 2kHz @ 72MHz clock
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 2000) - 1;
  #endif
	TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);
	
	/* Enable counter */
	TIM_Cmd(TIM15, ENABLE);
	TIM_Cmd(TIM17, ENABLE);
}

void delay_us(uint16_t cnt) 
{
	uint16_t start = micros();
	while((uint16_t)(micros() - start) <= cnt);
}

void delay_ms(uint16_t cnt) 
{
	uint16_t start = millis();
	while((uint16_t)(millis() - start) <= cnt);
}

// PWM's
// D3 - PA1 - TIM2_CH2
// D5 - PB6 - TIM4_CH1
// D6 - PA11- TIM1_CH4
// D10- PA7 - TIM3_CH2
// D11- PA6 - TIM3_CH1
