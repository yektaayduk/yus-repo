/******************************************
 * project: STM32F103RB FreeRTOS Template
 *    file: main.c
 ******************************************/

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

/* led connected to a gpio pin */
#define LED_PIN    GPIO_Pin_9
#define LED_PORT   GPIOB

#define LED_TASK_PRIORITY    ( tskIDLE_PRIORITY + 2 )
#define LED_DELAY			(( portTickType ) 200 / portTICK_RATE_MS ) // 200ms

/* user functions */
static void prvSetupHardware( void );
static void prvLedTask( void *pvParameters );

int main()
{
    /* initialize hardware */
    prvSetupHardware();

    /* create the example led task */
    xTaskCreate(                           /* Create a new task and add it to the list of tasks that are ready to run */
            prvLedTask,                    /* Pointer to the task entry function */
           (signed portCHAR*)"LED Blinker",/* A descriptive name for the task */
            configMINIMAL_STACK_SIZE,      /* The size of the task stack specified as the number of variables the stack can hold */
            NULL,                          /* Pointer that will be used as the parameter for the task being created */
            LED_TASK_PRIORITY,             /* The priority at which the task should run */
            NULL                           /* Used to pass back a handle by which the created task can be referenced */
            );                             /* returns pdPASS (1) if successful */

    /* create other tasks here */
    // other xTaskCreate()'s

    /* start the scheduler. */
    vTaskStartScheduler();  /* Starts the real time kernel tick processing  */

    /* should not get here */
    return 0;
}

static void prvSetupHardware( void )
{
    /* Enable GPIOA, GPIOB, GPIOC and AFIO clocks */
    RCC_APB2PeriphClockCmd(    RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
            | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE );

    /* Set the Vector Table base address at 0x08000000 */
    NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );

    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

    /* Configure HCLK clock as SysTick clock source. */
    SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
}

static void prvLedTask( void *pvParameters )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    portTickType xLastExecutionTime;

    /* set pin output mode */
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    /* Initialise the xLastExecutionTime variable on task entry. */
    xLastExecutionTime = xTaskGetTickCount();

    for( ;; )
    {
        GPIO_SetBits(LED_PORT, LED_PIN);    // set pin high
        vTaskDelayUntil( &xLastExecutionTime, LED_DELAY );
        GPIO_ResetBits(LED_PORT, LED_PIN);  // set pin low
        vTaskDelayUntil( &xLastExecutionTime, LED_DELAY );
    }
}

