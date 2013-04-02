
#include <stdint.h>
#include <stdbool.h>

#include <inc/hw_gpio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/rom.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include "gpio.h"

static const uint32_t port_base[] =
{
	GPIO_PORTA_BASE,
	GPIO_PORTB_BASE,
	GPIO_PORTC_BASE,
	GPIO_PORTD_BASE,
	GPIO_PORTE_BASE,
	GPIO_PORTF_BASE,
	//GPIO_PORTG_BASE,
	//GPIO_PORTH_BASE,
	//GPIO_PORTJ_BASE,
};

void bsp_gpio_init(void)
{
	/* Enable GPIO peripherals */
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
	/* unlock NMI pins: PF0 (SW2 button) and PD7 */
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= (1<<0);
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= (1<<7);
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
	
	/* initialize push buttons */
	pinMode(BUTTON_SW2_PIN, INPUT_PULLUP);
	pinMode(BUTTON_SW1_PIN, INPUT_PULLUP);
}

void pinMode(uint8_t pin, uint8_t mode)
{
	uint32_t port = port_base[(pin>>4)-0xA];
	uint8_t bit = 1<<(pin&0xF);
	switch(mode)
	{
	case INPUT_FLOATING:
		ROM_GPIOPinTypeGPIOInput(port, bit);
		break;
	case INPUT_PULLUP:
		ROM_GPIODirModeSet(port, bit, GPIO_DIR_MODE_IN);
		ROM_GPIOPadConfigSet(port, bit, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
		break;
	case INPUT_PULLDOWN:
		ROM_GPIODirModeSet(port, bit, GPIO_DIR_MODE_IN);
		ROM_GPIOPadConfigSet(port, bit, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
		break;
	case OUTPUT_OPENDRAIN:
		ROM_GPIOPinTypeGPIOOutputOD(port, bit);
	case OUTPUT_PUSHPULL:
		ROM_GPIOPinTypeGPIOOutput(port, bit);
		break;
	}
}

void setPinHigh(uint8_t pin)
{
	uint8_t bit = 1<<(pin&0xF);
	ROM_GPIOPinWrite(port_base[(pin>>4)-0xA], bit, bit);
}

void setPinLow(uint8_t pin)
{
	ROM_GPIOPinWrite(port_base[(pin>>4)-0xA], 1<<(pin&0xF), 0);
}

void setPinState(uint8_t pin, bool state)
{
	uint8_t bit = 1<<(pin&0xF);
	ROM_GPIOPinWrite(port_base[(pin>>4)-0xA], bit, state? bit:0);
}

bool getPinState(uint8_t pin)
{
	return ROM_GPIOPinRead(port_base[(pin>>4)-0xA], 1<<(pin&0xF))? true:false;
}
