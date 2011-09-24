/*******************************
 * project: STM32 PushButton
 *    file: main.c
 *******************************/

#include "stm32f10x.h"

/* push buttons on GPIOC */
#define PB1			GPIO_Pin_0
#define PB2			GPIO_Pin_1
#define PB_PORT		GPIOC

/* leds on GPIOB*/
#define LED1		GPIO_Pin_8
#define LED2		GPIO_Pin_9
#define LED_PORT	GPIOB

int main()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* enable clock on GPIOB & GPIOC peripherals */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    /* input mode for push buttons*/
    GPIO_InitStructure.GPIO_Pin = PB1 | PB2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(PB_PORT, &GPIO_InitStructure);

    /* output mode for leds*/
    GPIO_InitStructure.GPIO_Pin = LED1 | LED2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    while(1)
    {
        // "connect" PB1 to LED1
        if( GPIO_ReadInputDataBit(PB_PORT, PB1) == Bit_SET)
            GPIO_SetBits(LED_PORT, LED1);	// set pin high
        else
            GPIO_ResetBits(LED_PORT, LED1);	// set pin low
        // "connect" PB2 to LED2
        if( GPIO_ReadInputDataBit(PB_PORT, PB2) == Bit_SET)
            GPIO_SetBits(LED_PORT, LED2);	// set pin high
        else
            GPIO_ResetBits(LED_PORT, LED2);	// set pin low
    }
    return 0;
}
