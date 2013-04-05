
#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

// 32-bit timer base
#define DELAY_TIMER_BASE	WTIMER4_BASE

// 1MHz
#define micros()	( 0xFFFFFFFF - HWREG(DELAY_TIMER_BASE + TIMER_O_TAR) )
// 2kHz / 2
#define millis()	( (0xFFFFFFFF - HWREG(DELAY_TIMER_BASE + TIMER_O_TBR)) >> 1 )

#ifdef __cplusplus
extern "C" {
#endif

void delay_us(uint32_t count);
void delay_ms(uint32_t count);

#ifdef __cplusplus
}
#endif

#endif	// __BSP_TIMER_H__
