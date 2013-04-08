#include <lm4f120xl.h>
#include <FreeRTOS.h>
#include <RGB.h>

#define PRIORITY_SWITCH_TASK    2
#define PRIORITY_LED_TASK       1

#define SWITCHTASKSTACKSIZE		128         // Stack size in words
#define SWITCH_DELAY			50

// The stack size for the LED toggle task.
#define LEDTASKSTACKSIZE		128         // Stack size in words
#define LED_ITEM_SIZE			sizeof(unsigned char)
#define LED_QUEUE_SIZE			5
#define LED_TOGGLE_DELAY		250	// Default LED toggle delay value

#define LEFT_BUTTON		1
#define RIGHT_BUTTON	2
#define ALL_BUTTONS		(LEFT_BUTTON|RIGHT_BUTTON)

xQueueHandle g_pLEDQueue;
// [R, G, B] range is 0 to 0xFFFF per color.
static unsigned long g_ulColors[3] = { 0x0000, 0x0000, 0x0000 };
static unsigned char g_ucColorsIndx;

xSemaphoreHandle g_pUARTSemaphore;

static void LEDTask(void *pvParameters)
{
	portTickType ulWakeTime;
	unsigned long ulLEDToggleDelay;
	unsigned char cMessage;
	
	// Initialize the LED Toggle Delay to default value.
	ulLEDToggleDelay = LED_TOGGLE_DELAY;
	
	// Get the current tick count.
	ulWakeTime = xTaskGetTickCount();
	
	// Loop forever.
	while(1)
	{
		// Read the next message, if available on queue.
		if(xQueueReceive(g_pLEDQueue, &cMessage, 0) == pdPASS)
		{
			// If left button, update to next LED.
			if(cMessage == LEFT_BUTTON)
			{
				// Update the LED buffer to turn off the currently working.
				g_ulColors[g_ucColorsIndx] = 0x0000;
				// Update the index to next LED
				if(++g_ucColorsIndx > 2) g_ucColorsIndx = 0;
				// Update the LED buffer to turn on the newly selected LED.
				g_ulColors[g_ucColorsIndx] = 0x8000;
				// Configure the new LED settings.
				RGBColorSet(g_ulColors);
				
				// Guard UART from concurrent access. Print the currently blinking LED.
				xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
				Serial.print("%s Led is blinking.\n", g_ucColorsIndx? ( g_ucColorsIndx==1? "GREEN":"BLUE" ):"RED" );
				xSemaphoreGive(g_pUARTSemaphore);
			}
			// If right button, update delay time between toggles of led.
			if(cMessage == RIGHT_BUTTON)
			{
				ulLEDToggleDelay *= 2;
				if(ulLEDToggleDelay > 1000)
				{
					ulLEDToggleDelay = LED_TOGGLE_DELAY / 2;
				}
				// Guard UART from concurrent access. Print the currently blinking frequency.
				xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
				Serial.print("Led blinking period is %d ms.\n", (ulLEDToggleDelay * 2));
				xSemaphoreGive(g_pUARTSemaphore);
			}
		}
		// Turn on the LED.
		RGBEnable();
		// Wait for the required amount of time.
		vTaskDelayUntil(&ulWakeTime, ulLEDToggleDelay / portTICK_RATE_MS);
		// Turn off the LED.
		RGBDisable();
		// Wait for the required amount of time.
		vTaskDelayUntil(&ulWakeTime, ulLEDToggleDelay / portTICK_RATE_MS);
	}
}

unsigned long LEDTaskInit(void)
{
	// Initialize the GPIOs and Timers that drive the three LEDs.
	RGBInit(1);
	RGBIntensitySet(0.3f);
	
	// Turn on the Green LED
	g_ucColorsIndx = 0;
	g_ulColors[g_ucColorsIndx] = 0x8000;
	RGBColorSet(g_ulColors);
	
	// Print the current toggling LED and frequency.
	Serial.print("\n%s Led is blinking.\n", g_ucColorsIndx? ( g_ucColorsIndx==1? "GREEN":"BLUE" ):"RED" );
	Serial.print("Led blinking period is %d ms.\n", (LED_TOGGLE_DELAY * 2));
	
	// Create a queue for sending messages to the LED task.
	g_pLEDQueue = xQueueCreate(LED_QUEUE_SIZE, LED_ITEM_SIZE);
	
	// Create the LED task.
	if(xTaskCreate(LEDTask, (signed portCHAR *)"LED", LEDTASKSTACKSIZE, NULL,
		tskIDLE_PRIORITY + PRIORITY_LED_TASK, NULL) != pdTRUE)
	{
		return(1);
	}
	// Success.
	return(0);
}

static void SwitchTask(void *pvParameters)
{
	portTickType ulLastTime;
	unsigned char ucCurButtonState, ucPrevButtonState;
	unsigned char ucMessage;
	
	ucCurButtonState = ucPrevButtonState = 0;
	// Get the current tick count.
	ulLastTime = xTaskGetTickCount();
	
	// Loop forever.
	while(1)
	{
		// Poll the state of the buttons.
		ucCurButtonState = ~(getPinState(BUTTON_SW1_PIN) + (getPinState(BUTTON_SW2_PIN)<<1));
		
		// Check if previous debounced state is equal to the current state.
		if(ucCurButtonState != ucPrevButtonState)
		{
			ucPrevButtonState = ucCurButtonState;
			// Check to make sure the change in state is due to button press and not due to button release.
			if((ucCurButtonState & ALL_BUTTONS) != 0)
			{
				if((ucCurButtonState & ALL_BUTTONS) == LEFT_BUTTON)
				{
					ucMessage = LEFT_BUTTON;
					
					// Guard UART from concurrent access.
					xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
					Serial.print("Left Button is pressed.\n");
					xSemaphoreGive(g_pUARTSemaphore);
				}
				else if((ucCurButtonState & ALL_BUTTONS) == RIGHT_BUTTON)
				{
					ucMessage = RIGHT_BUTTON;
					xSemaphoreTake(g_pUARTSemaphore, portMAX_DELAY);
					Serial.print("Right Button is pressed.\n");
					xSemaphoreGive(g_pUARTSemaphore);
				}
				// Pass the value of the button pressed to LEDTask.
				if(xQueueSend(g_pLEDQueue, &ucMessage, portMAX_DELAY) != pdPASS)
				{
					// Error. The queue should never be full. If so print the error message on UART and wait for ever.
					Serial.print("\nQueue full. This should never happen.\n");
					while(1) {}
				}
			}
		}
		// Wait for the required amount of time to check back.
		vTaskDelayUntil(&ulLastTime, SWITCH_DELAY / portTICK_RATE_MS);
	}
}


int main(void)
{
	Serial.begin(115200);
	Serial.print("Welcome to the Stellaris EK-LM4F120 FreeRTOS Demo!\n");
	
	// Create a mutex to guard the UART.
	g_pUARTSemaphore = xSemaphoreCreateMutex();
	
	// Create the LED task.
	if(LEDTaskInit() != 0)
	{
		while(1) {}
	}
	
	// Create the switch task.
	if(xTaskCreate(SwitchTask, (signed portCHAR *)"Switch",
		SWITCHTASKSTACKSIZE, NULL, tskIDLE_PRIORITY +
		PRIORITY_SWITCH_TASK, NULL) != pdTRUE)
	{
		while(1) {}
	}
	
	vTaskStartScheduler();
	
	while(true)
	{
	}
}
