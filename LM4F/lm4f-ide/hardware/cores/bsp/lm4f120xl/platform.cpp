
#include <stdlib.h>
#include <sys/types.h>

#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"

extern "C" { void bsp_timer_init(void) ; }
extern "C" { void bsp_gpio_init(void) ; }
extern "C" { void bsp_init(void) ; }

void bsp_init(void)
{
	/* set 80MHz clock */
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	
	/* Enable GPIO peripherals */
	bsp_gpio_init();
	
	/* Enable delay counters */
	bsp_timer_init();
}

/*
 * C++ tweaks
 * http://andybrown.me.uk/wk/2011/12/28/stm32plus-a-c-library-for-stm32-development
 */

namespace __gnu_cxx {
 
  void __verbose_terminate_handler() {
    for(;;);
  }
}
 
 extern "C" void __cxa_pure_virtual() {
  for(;;);
}
 
void *operator new(size_t size) {
  return malloc(size);
}
 
void *operator new[](size_t size) {
  return malloc(size);
}
 
void operator delete(void *p) {
  free(p);
}
 
void operator delete[](void *p) {
  free(p);
}
