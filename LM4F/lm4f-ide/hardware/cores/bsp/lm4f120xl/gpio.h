
#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

/*** J1 ***/
// 3V3
#define PB5		(0xB0 + 5)
#define PB0		(0xB0 + 0)
#define PB1		(0xB0 + 1)
#define PE4		(0xE0 + 4)
#define PE5		(0xE0 + 5)
#define PB4		(0xB0 + 4)
#define PA5		(0xA0 + 5)
#define PA6		(0xA0 + 6)
#define PA7		(0xA0 + 7)

/*** J3 ***/
// VBUS
// GND
#define PD0		(0xD0 + 0)
#define PD1		(0xD0 + 1)
#define PD2		(0xD0 + 2)
#define PD3		(0xD0 + 3)
#define PE1		(0xE0 + 1)
#define PE2		(0xE0 + 2)
#define PE3		(0xE0 + 3)
//#define PF1	(0xF0 + 1) // RED_LED

/*** J4 ***/
//#define PF2	(0xF0 + 2) // BLUE_LED
//#define PF3	(0xF0 + 3) // GREEN_LED
#define PB3		(0xB0 + 3)
#define PC4		(0xC0 + 4)
#define PC5		(0xC0 + 5)
#define PC6		(0xC0 + 6)
#define PC7		(0xC0 + 7)
#define PD6		(0xD0 + 6)
#define PD7		(0xD0 + 7)
//#define PF4	(0xF0 + 4) // PUSH1

/*** J2 ***/
// GND
#define PB2		(0xB0 + 2)
#define PE0		(0xE0 + 0)
//#define PF0	(0xF0 + 0) // PUSH2
// RST
#define PB7		(0xB0 + 7)
#define PB6		(0xB0 + 6)
#define PA4		(0xA0 + 4)
#define PA3		(0xA0 + 3)
#define PA2		(0xA0 + 2)


// LED's
#define LED_RED_PIN		(0xF0 + 1)
#define LED_GREEN_PIN	(0xF0 + 3)
#define LED_BLUE_PIN	(0xF0 + 2)
// push buttons
#define BUTTON_SW1_PIN	(0xF0 + 4)
#define BUTTON_SW2_PIN	(0xF0 + 0)


// pin modes
//#define INPUT_ANALOG		0
#define INPUT_FLOATING		1
#define INPUT_PULLUP		2
#define INPUT_PULLDOWN		3
#define OUTPUT_OPENDRAIN	4
#define OUTPUT_PUSHPULL		5

#define INPUT	INPUT_FLOATING
#define OUTPUT	OUTPUT_PUSHPULL

#ifdef __cplusplus
extern "C" {
#endif

void pinMode(uint8_t pin, uint8_t mode);
void setPinHigh(uint8_t pin);
void setPinLow(uint8_t pin);
void setPinState(uint8_t pin, bool state);
bool getPinState(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif	// __BSP_GPIO_H__
