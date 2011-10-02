/******************************************
 * project: STM32F103RB FreeRTOS Template
 *    file: main.c
 ******************************************/

#include "stm32f10x.h"

/* led connected to a gpio pin */
#define LED_PIN    GPIO_Pin_9
#define LED_PORT   GPIOB

/* user functions */
void delay(unsigned long count);

int main()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* enable clock on GPIOB peripheral */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /* set pin output mode */
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    while(1)
    {
        GPIO_SetBits(LED_PORT, LED_PIN);	// set pin high
        delay(700000);
        GPIO_ResetBits(LED_PORT, LED_PIN);	// set pin low
        delay(700000);
    }
    return 0;
}

void delay(unsigned long count)
{
    while(count--);
}
