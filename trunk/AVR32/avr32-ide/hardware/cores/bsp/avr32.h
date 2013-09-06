
#ifndef YUS_AVR_H
#define YUS_AVR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <avr32/io.h>
#include <compiler.h>
#include <power_clocks_lib.h>
#include <gpio.h>
#include <usart.h>

#define TARGET_DFLL_FREQ_HZ   96000000  // DFLL target frequency, in Hz
#define TARGET_MCUCLK_FREQ_HZ 12000000  // MCU clock target frequency, in Hz
#define TARGET_PBACLK_FREQ_HZ 12000000  // PBA clock target frequency, in Hz

#ifdef __cplusplus
}
#endif

#endif // YUS_AVR_H
