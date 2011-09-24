/*******************************
 * project: STM32 LED Blinker
 *    file: main.c
 *******************************/

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

/* led connected to a gpio pin */
#define LED_PIN      9          // pin 9
#define OUTPUT_MODE (GPIO_Mode_Out_PP|GPIO_Speed_50MHz) // push-pull + 50MHz

/* user functions */
void delay(unsigned long count);

int main()
{
    /* enable clock on GPIOB peripheral */
    RCC->APB2ENR = RCC_APB2ENR_IOPBEN;
    
    /* set pin output mode */
    //GPIOB->CRL |= OUTPUT_MODE << ((LED_PIN) << 2); // if pins 0 to 7
    GPIOB->CRH |= OUTPUT_MODE << ((LED_PIN-8) << 2); // if pins 8 to 15

    while(1)
    {
        GPIOB->BSRR = 1<<LED_PIN;  // set pin high
        delay(200000);
        GPIOB->BRR  = 1<<LED_PIN;  // set pin low
        delay(200000);
    }
    return 0;
}

void delay(unsigned long count)
{
    while(count--);
}
