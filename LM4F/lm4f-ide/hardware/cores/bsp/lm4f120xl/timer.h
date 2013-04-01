
#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

void delay_us(uint16_t count);
void delay_ms(uint16_t count);

// 1MHz
#define micros()	((uint16_t)0)
// 1kHz
#define millis()	((uint16_t)0)

#ifdef __cplusplus
}
#endif

#endif	// __BSP_TIMER_H__
