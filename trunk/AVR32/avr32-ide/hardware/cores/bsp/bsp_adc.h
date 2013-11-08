
#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <avr32.h>

bool adcMode(uint32_t pin);
uint32_t adcRead(uint32_t pin);

#define analogRead(pin)	adcRead(pin)

#ifdef __cplusplus
}
#endif

#endif	// __BSP_ADC_H__
