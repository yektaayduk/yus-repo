
#include <inc/hw_memmap.h>
#include <inc/hw_timer.h>
#include <inc/hw_types.h>
#include <driverlib/rom.h>
#include <driverlib/sysctl.h>
#include <driverlib/timer.h>

#include "timer.h"

void bsp_timer_init(void)
{
#if(DELAY_TIMER_BASE==WTIMER0_BASE)
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
#elif(DELAY_TIMER_BASE==WTIMER1_BASE)
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER1);
#elif(DELAY_TIMER_BASE==WTIMER2_BASE)
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
#elif(DELAY_TIMER_BASE==WTIMER3_BASE)
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER3);
#elif(DELAY_TIMER_BASE==WTIMER4_BASE)
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER4);
#elif(DELAY_TIMER_BASE==WTIMER5_BASE)
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER5);
#else
	#error delay timer base not supported!
#endif
	// Periodic (prescaler works only on countdown)
	ROM_TimerConfigure(DELAY_TIMER_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC | TIMER_CFG_B_PERIODIC);
	// TIMER_4A -> 1MHz
	ROM_TimerPrescaleSet(DELAY_TIMER_BASE, TIMER_A, ROM_SysCtlClockGet()/1000000 - 1);
	ROM_TimerLoadSet(DELAY_TIMER_BASE, TIMER_A, 0xFFFFFFFF); // full 32-bit range
	// TIMER_4B -> 2kHz (note: 16-bit prescaler only, i.e. ~1.22kHz min at 80MHz clock)
	ROM_TimerPrescaleSet(DELAY_TIMER_BASE, TIMER_B, ROM_SysCtlClockGet()/2000 - 1);
	ROM_TimerLoadSet(DELAY_TIMER_BASE, TIMER_B, 0xFFFFFFFF); // full 32-bit range
	// Enable the timers
	ROM_TimerEnable(DELAY_TIMER_BASE, TIMER_BOTH);
}

void delay_us(unsigned long count)
{
	volatile unsigned long start = HWREG(DELAY_TIMER_BASE + TIMER_O_TAR);
	while((unsigned long)(start - HWREG(DELAY_TIMER_BASE + TIMER_O_TAR)) <= count);
}

void delay_ms(unsigned long count)
{
	volatile unsigned long start = HWREG(DELAY_TIMER_BASE + TIMER_O_TBR);
	while((unsigned long)(start - HWREG(DELAY_TIMER_BASE + TIMER_O_TBR)) <= (count<<1));
}
