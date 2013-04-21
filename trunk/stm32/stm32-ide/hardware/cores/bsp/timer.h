
#ifndef ARM_MCU_TIMER_H
#define ARM_MCU_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

void delay_us(uint16_t cnt);
void delay_ms(uint16_t cnt);

// TIM15 - 1MHz
#define micros()	((uint16_t)(TIM15->CNT))

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || (defined STM32F10X_HD_VL)
  // TIM17 - 1kHz @ 24MHZ clock
  #define millis()	((uint16_t)(TIM17->CNT))
#else
  // TIM17 - 2kHz @ 72MHZ clock
  #define millis()	((uint16_t)(TIM17->CNT)>>1)
#endif

#ifdef __cplusplus
}
#endif

#endif	// ARM_MCU_TIMER_H
