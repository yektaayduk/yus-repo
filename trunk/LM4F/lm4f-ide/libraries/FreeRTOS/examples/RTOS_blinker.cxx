#include <lm4f120xl.h>
#include <FreeRTOS.h>

#define LEDTASKSTACKSIZE        128
#define PRIORITY_LED_TASK       1
#define LED_TOGGLE_DELAY        500

static void LEDTask(void *pvParameters)
{
	portTickType ulWakeTime = xTaskGetTickCount();
	pinMode(LED_BLUE_PIN, OUTPUT);
	while(1)
	{
		togglePin(LED_BLUE_PIN);
		vTaskDelayUntil(&ulWakeTime, LED_TOGGLE_DELAY / portTICK_RATE_MS);
	}
}

int main(void)
{
	xTaskCreate(LEDTask, (signed portCHAR *)"LED", LEDTASKSTACKSIZE, NULL,
			tskIDLE_PRIORITY + PRIORITY_LED_TASK, NULL);
	
	vTaskStartScheduler();
	
	while(true)	{ }
}
