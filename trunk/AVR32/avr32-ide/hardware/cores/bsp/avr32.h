
#ifndef YUS_AVR_H
#define YUS_AVR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <avr32/io.h>
#include <compiler.h>
#include <power_clocks_lib.h>

#include <acifb.h>
#include <adcifb.h>
#include <ast.h>
#include <eic.h>
#include <flashcdw.h>
#include <freqm.h>
#include <gpio.h>
#include <intc.h>
#include <pdca.h>
#include <scif_uc3l.h>
#include <spi.h>
#include <tc.h>
#include <twim.h>
//#include <twis.h>
#include <usart.h>
#include <wdt.h>

#ifdef __cplusplus
}
#endif


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include <bsp_clock.h>
#include <bsp_gpio.h>
#include <bsp_adc.h>
#include <bsp_uart.h>

#endif // YUS_AVR_H
