/******************************************
 * project: STM32F103RB FreeRTOS Demo 01
 *    file: main.c
 ******************************************/

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "xuart.h"

/* led connected to gpio pin */
#define LED1        GPIO_Pin_8
#define LED2        GPIO_Pin_9
#define LED_PORT    GPIOB

/* push buttons on GPIOC */
#define PB1         GPIO_Pin_0
#define PB2			GPIO_Pin_1
#define PB_PORT     GPIOC

#define LED_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define UART_TASK_PRIORITY    ( tskIDLE_PRIORITY + 1 )
#define BUTTON_TASK_PRIORITY  ( tskIDLE_PRIORITY + 3 )
#define PRINT_TASK_PRIORITY   ( tskIDLE_PRIORITY + 3 )

#define LED_DELAY            (( portTickType ) 200 / portTICK_RATE_MS ) // 200ms
#define BUTTON_DELAY         (( portTickType ) 50 / portTICK_RATE_MS ) // 50ms
#define PRINT_DELAY          (( portTickType ) 200 / portTICK_RATE_MS ) // 200ms

/* user functions */
static void prvSetupHardware( void );
static void prvLedTask( void *pvParameters );
static void prvUartTask( void *pvParameters );
static void prvButtonTask( void *pvParameters );
static void prvPrintTask( void *pvParameters );

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

    /* create uart task */
    xTaskCreate(
            prvUartTask,
           (signed portCHAR*)"xUART test",
            configMINIMAL_STACK_SIZE + 64,
            NULL,
            UART_TASK_PRIORITY,
            NULL );

    /* create push button task */
    xTaskCreate(
            prvButtonTask,
           (signed portCHAR*)"Push Button test",
            configMINIMAL_STACK_SIZE,
            NULL,
            BUTTON_TASK_PRIORITY,
            NULL );

    /* create printf task */
	xTaskCreate(
			prvPrintTask,
		   (signed portCHAR*)"Print test",
			configMINIMAL_STACK_SIZE,
			NULL,
			PRINT_TASK_PRIORITY,
			NULL );

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

    /* Enable UART1 w/ 115200 baud */
    xuart_init(115200);
    xprintf("hello stm%d!\r\n", 32);
}

static void prvLedTask( void *pvParameters )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    portTickType xLastExecutionTime;

    /* set pin output mode */
    GPIO_InitStructure.GPIO_Pin = LED1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    /* Initialise the xLastExecutionTime variable on task entry. */
    xLastExecutionTime = xTaskGetTickCount();

    for( ;; )
    {
        GPIO_SetBits(LED_PORT, LED1);    // set pin high
        vTaskDelayUntil( &xLastExecutionTime, LED_DELAY );
        GPIO_ResetBits(LED_PORT, LED1);  // set pin low
        vTaskDelayUntil( &xLastExecutionTime, LED_DELAY );
    }
}

static void prvUartTask( void *pvParameters )
{
    char buff[50];

    for( ;; )
    {
        xprintf("what's your name?: ");
        xgets(buff, 50);
        if(buff[0])
            xprintf("hi there, %s!\n", buff);
    }
}

static void prvButtonTask( void *pvParameters )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    portTickType xLastExecutionTime;

    /* input mode for push button 2*/
    GPIO_InitStructure.GPIO_Pin = PB2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(PB_PORT, &GPIO_InitStructure);

    /* output mode for led 2*/
    GPIO_InitStructure.GPIO_Pin = LED2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    xLastExecutionTime = xTaskGetTickCount();

    for( ;; )
	{
    	vTaskDelayUntil( &xLastExecutionTime, BUTTON_DELAY );
    	// "connect" PB2 to LED2
		if( GPIO_ReadInputDataBit(PB_PORT, PB2) == Bit_SET)
			GPIO_SetBits(LED_PORT, LED2);	// set pin high
		else
			GPIO_ResetBits(LED_PORT, LED2);	// set pin low
	}
}

static void prvPrintTask( void *pvParameters )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    portTickType xLastExecutionTime;

    /* input mode for push button 1*/
    GPIO_InitStructure.GPIO_Pin = PB1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(PB_PORT, &GPIO_InitStructure);

    xLastExecutionTime = xTaskGetTickCount();

    for( ;; )
	{
    	vTaskDelayUntil( &xLastExecutionTime, PRINT_DELAY >> 3 );
    	// if PB1 is pressed, print some messages
		if( GPIO_ReadInputDataBit(PB_PORT, PB1) == Bit_RESET){
			xprintf("\n you pressed me! \n");
			vTaskDelayUntil( &xLastExecutionTime, PRINT_DELAY );
		}
	}
}

