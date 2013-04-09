/*
	USB HID Keyboard
	- press SW1 button to send some string
	- on another keyboard, press 'Caps Lock' to toggle the BLUE Led
*/
#include <lm4f120xl.h>
#include <usblib.h>
#include <usblib/usbhid.h>
#include <usblib/usb-ids.h>
#include <usblib/device/usbdevice.h>
#include <usblib/device/usbdhid.h>
#include <usblib/device/usbdhidkeyb.h>

// The languages supported by this device.
const unsigned char g_pLangDescriptor[] =
{
    4,
    USB_DTYPE_STRING,
    USBShort(USB_LANG_EN_US)
};

// The manufacturer string.
const unsigned char g_pManufacturerString[] =
{
    (17 + 1) * 2,
    USB_DTYPE_STRING,
    'T', 0, 'e', 0, 'x', 0, 'a', 0, 's', 0, ' ', 0, 'I', 0, 'n', 0, 's', 0,
    't', 0, 'r', 0, 'u', 0, 'm', 0, 'e', 0, 'n', 0, 't', 0, 's', 0,
};

// The product string.
const unsigned char g_pProductString[] =
{
    (16 + 1) * 2,
    USB_DTYPE_STRING,
    'K', 0, 'e', 0, 'y', 0, 'b', 0, 'o', 0, 'a', 0, 'r', 0, 'd', 0, ' ', 0,
    'E', 0, 'x', 0,'a', 0,'m', 0, 'p', 0, 'l', 0, 'e', 0
};

// The serial number string.
const unsigned char g_pSerialNumberString[] =
{
    (8 + 1) * 2,
    USB_DTYPE_STRING,
    '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0
};

// The interface description string.
const unsigned char g_pHIDInterfaceString[] =
{
    (22 + 1) * 2,
    USB_DTYPE_STRING,
    'H', 0, 'I', 0, 'D', 0, ' ', 0, 'K', 0, 'e', 0, 'y', 0, 'b', 0,
    'o', 0, 'a', 0, 'r', 0, 'd', 0, ' ', 0, 'I', 0, 'n', 0, 't', 0,
    'e', 0, 'r', 0, 'f', 0, 'a', 0, 'c', 0, 'e', 0
};

// The configuration description string.
const unsigned char g_pConfigString[] =
{
    (26 + 1) * 2,
    USB_DTYPE_STRING,
    'H', 0, 'I', 0, 'D', 0, ' ', 0, 'K', 0, 'e', 0, 'y', 0, 'b', 0,
    'o', 0, 'a', 0, 'r', 0, 'd', 0, ' ', 0, 'C', 0, 'o', 0, 'n', 0,
    'f', 0, 'i', 0, 'g', 0, 'u', 0, 'r', 0, 'a', 0, 't', 0, 'i', 0,
    'o', 0, 'n', 0
};

// The descriptor string table.
const unsigned char * const g_pStringDescriptors[] =
{
    g_pLangDescriptor,
    g_pManufacturerString,
    g_pProductString,
    g_pSerialNumberString,
    g_pHIDInterfaceString,
    g_pConfigString
};

#define NUM_STRING_DESCRIPTORS (sizeof(g_pStringDescriptors) / sizeof(unsigned char *))

unsigned long KeyboardHandler(void *pvCBData, unsigned long ulEvent,
	unsigned long ulMsgData, void *pvMsgData);

// The HID keyboard device initialization and customization structures.
tHIDKeyboardInstance g_KeyboardInstance;

const tUSBDHIDKeyboardDevice g_sKeyboardDevice =
{
    USB_VID_STELLARIS,
    USB_PID_KEYBOARD,
    500,
    USB_CONF_ATTR_SELF_PWR | USB_CONF_ATTR_RWAKE,
    KeyboardHandler,
    (void *)&g_sKeyboardDevice,
    g_pStringDescriptors,
    NUM_STRING_DESCRIPTORS,
    &g_KeyboardInstance
};

// A mapping from the ASCII value received from the UART to the corresponding USB HID usage code.
static const char g_ppcKeyUsageCodes[][2] =
{
    { 0, HID_KEYB_USAGE_SPACE },                       //   0x20
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_1 },         // ! 0x21
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_FQUOTE },    // " 0x22
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_3 },         // # 0x23
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_4 },         // $ 0x24
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_5 },         // % 0x25
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_7 },         // & 0x26
    { 0, HID_KEYB_USAGE_FQUOTE },                      // ' 0x27
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_9 },         // ( 0x28
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_0 },         // ) 0x29
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_8 },         // * 0x2a
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_EQUAL },     // + 0x2b
    { 0, HID_KEYB_USAGE_COMMA },                       // , 0x2c
    { 0, HID_KEYB_USAGE_MINUS },                       // - 0x2d
    { 0, HID_KEYB_USAGE_PERIOD },                      // . 0x2e
    { 0, HID_KEYB_USAGE_FSLASH },                      // / 0x2f
    { 0, HID_KEYB_USAGE_0 },                           // 0 0x30
    { 0, HID_KEYB_USAGE_1 },                           // 1 0x31
    { 0, HID_KEYB_USAGE_2 },                           // 2 0x32
    { 0, HID_KEYB_USAGE_3 },                           // 3 0x33
    { 0, HID_KEYB_USAGE_4 },                           // 4 0x34
    { 0, HID_KEYB_USAGE_5 },                           // 5 0x35
    { 0, HID_KEYB_USAGE_6 },                           // 6 0x36
    { 0, HID_KEYB_USAGE_7 },                           // 7 0x37
    { 0, HID_KEYB_USAGE_8 },                           // 8 0x38
    { 0, HID_KEYB_USAGE_9 },                           // 9 0x39
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_SEMICOLON }, // : 0x3a
    { 0, HID_KEYB_USAGE_SEMICOLON },                   // ; 0x3b
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_COMMA },     // < 0x3c
    { 0, HID_KEYB_USAGE_EQUAL },                       // = 0x3d
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_PERIOD },    // > 0x3e
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_FSLASH },    // ? 0x3f
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_2 },         // @ 0x40
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_A },         // A 0x41
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_B },         // B 0x42
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_C },         // C 0x43
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_D },         // D 0x44
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_E },         // E 0x45
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_F },         // F 0x46
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_G },         // G 0x47
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_H },         // H 0x48
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_I },         // I 0x49
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_J },         // J 0x4a
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_K },         // K 0x4b
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_L },         // L 0x4c
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_M },         // M 0x4d
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_N },         // N 0x4e
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_O },         // O 0x4f
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_P },         // P 0x50
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_Q },         // Q 0x51
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_R },         // R 0x52
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_S },         // S 0x53
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_T },         // T 0x54
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_U },         // U 0x55
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_V },         // V 0x56
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_W },         // W 0x57
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_X },         // X 0x58
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_Y },         // Y 0x59
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_Z },         // Z 0x5a
    { 0, HID_KEYB_USAGE_LBRACKET },                    // [ 0x5b
    { 0, HID_KEYB_USAGE_BSLASH },                      // \ 0x5c
    { 0, HID_KEYB_USAGE_RBRACKET },                    // ] 0x5d
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_6 },         // ^ 0x5e
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_MINUS },     // _ 0x5f
    { 0, HID_KEYB_USAGE_BQUOTE },                      // ` 0x60
    { 0, HID_KEYB_USAGE_A },                           // a 0x61
    { 0, HID_KEYB_USAGE_B },                           // b 0x62
    { 0, HID_KEYB_USAGE_C },                           // c 0x63
    { 0, HID_KEYB_USAGE_D },                           // d 0x64
    { 0, HID_KEYB_USAGE_E },                           // e 0x65
    { 0, HID_KEYB_USAGE_F },                           // f 0x66
    { 0, HID_KEYB_USAGE_G },                           // g 0x67
    { 0, HID_KEYB_USAGE_H },                           // h 0x68
    { 0, HID_KEYB_USAGE_I },                           // i 0x69
    { 0, HID_KEYB_USAGE_J },                           // j 0x6a
    { 0, HID_KEYB_USAGE_K },                           // k 0x6b
    { 0, HID_KEYB_USAGE_L },                           // l 0x6c
    { 0, HID_KEYB_USAGE_M },                           // m 0x6d
    { 0, HID_KEYB_USAGE_N },                           // n 0x6e
    { 0, HID_KEYB_USAGE_O },                           // o 0x6f
    { 0, HID_KEYB_USAGE_P },                           // p 0x70
    { 0, HID_KEYB_USAGE_Q },                           // q 0x71
    { 0, HID_KEYB_USAGE_R },                           // r 0x72
    { 0, HID_KEYB_USAGE_S },                           // s 0x73
    { 0, HID_KEYB_USAGE_T },                           // t 0x74
    { 0, HID_KEYB_USAGE_U },                           // u 0x75
    { 0, HID_KEYB_USAGE_V },                           // v 0x76
    { 0, HID_KEYB_USAGE_W },                           // w 0x77
    { 0, HID_KEYB_USAGE_X },                           // x 0x78
    { 0, HID_KEYB_USAGE_Y },                           // y 0x79
    { 0, HID_KEYB_USAGE_Z },                           // z 0x7a
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_LBRACKET },  // { 0x7b
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_BSLASH },    // | 0x7c
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_RBRACKET },  // } 0x7d
    { HID_KEYB_LEFT_SHIFT, HID_KEYB_USAGE_BQUOTE },    // ~ 0x7e
	{ 0, HID_KEYB_USAGE_ENTER },                       // Enter 0x7f
};

static volatile tBoolean g_bUSBConfigured = false;
volatile tBoolean g_bConnected = false;
volatile tBoolean g_bSuspended = false;

#define MAX_SEND_DELAY          50

typedef enum
{
    STATE_UNCONFIGURED,
	STATE_IDLE,
	STATE_SENDING
}eKeyboardState_t;
volatile eKeyboardState_t g_eKeyboardState = STATE_UNCONFIGURED;

// Sends a string of characters via the USB HID keyboard interface.
void SendString(char *pcStr);
// Wait for a period of time for the state to become idle.
tBoolean WaitForSendIdle(unsigned long ulTimeoutTicks);

int main(void)
{
	tBoolean bLastSuspend;
	unsigned long ulMillis;
	
	Serial.begin(115200);
	Serial.print("\nStellaris USB KeyBoard device example\n\n");
	
	// Enable the GPIO pins for the LED
	pinMode(LED_BLUE_PIN, OUTPUT);
	pinMode(LED_GREEN_PIN, OUTPUT);
	
	// Not configured initially.
	g_bUSBConfigured = false;
	
	// Enable the GPIO peripheral used for USB, and configure the USB pins
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	
	// Tell the user what we are up to.
	Serial.print("Configuring USB...\n");
	
	// Set the USB stack mode to Device mode with VBUS monitoring.
	USBStackModeSet(0, USB_MODE_FORCE_DEVICE, 0);
	
	// Pass our device information to the USB HID device class driver,
	// initialize the USB controller and connect the device to the bus.
	USBDHIDKeyboardInit(0, &g_sKeyboardDevice);
	
	while(true)
	{
		// Tell the user what we are doing and provide some basic instructions.
		Serial.print("Waiting for host...\n");
		
		// Wait here until USB device is connected to a host.
		while(!g_bConnected) {}
		
		// Update the status.
		Serial.print("Host connected...\n");
		
		// Enter the idle state.
		g_eKeyboardState = STATE_IDLE;
		
		// Assume that the bus is not currently suspended if we have just been configured.
		bLastSuspend = false;
		
		while(g_bConnected)
		{
			// Remember the current time.
			ulMillis = millis();
			
			// Has the suspend state changed since last time we checked?
			if(bLastSuspend != g_bSuspended)
			{
				// Yes - the state changed so update the display.
				bLastSuspend = g_bSuspended;
				if(bLastSuspend){
					Serial.print("Bus suspended...\n");
					setPinLow(LED_GREEN_PIN);
				}
				else{
					Serial.print("Host connected...\n");
					setPinHigh(LED_GREEN_PIN);
				}
			}
			// See if the button was just pressed.
			if( !getPinState(BUTTON_SW1_PIN) )
			{
				// If the bus is suspended then resume it.  Otherwise, send the message
				if(g_bSuspended) {
					USBDHIDKeyboardRemoteWakeupRequest( (void *)&g_sKeyboardDevice);
				}
				else{
					SendString((char *)"Make the Switch to TI Microcontrollers!\n");
				}
			}
			while(millis()-ulMillis < 10)
			{
			}
		}
	}
}

unsigned long KeyboardHandler(void *pvCBData, unsigned long ulEvent,
	unsigned long ulMsgData, void *pvMsgData)
{
	switch (ulEvent)
	{
	// The host has connected to us and configured the device.
	case USB_EVENT_CONNECTED:
		g_bConnected = true;
		g_bSuspended = false;
		break;
	// The host has disconnected from us.
	case USB_EVENT_DISCONNECTED:
		g_bConnected = false;
		break;
	// We receive this event every time the host acknowledges transmission of a report.
	case USB_EVENT_TX_COMPLETE:
		g_eKeyboardState = STATE_IDLE;
		break;
	// This event indicates that the host has suspended the USB bus.
	case USB_EVENT_SUSPEND:
		g_bSuspended = true;
		break;
	// This event signals that the host has resumed signalling on the bus.
	case USB_EVENT_RESUME:
		g_bSuspended = false;
		break;
	case USBD_HID_KEYB_EVENT_SET_LEDS:
		setPinState(LED_BLUE_PIN, (ulMsgData & HID_KEYB_CAPS_LOCK)? true:false);
		break;
	default:
		break;
	}
	return 0;
}

void SendString(char *pcStr)
{
	unsigned long ulChar;
	while(*pcStr)
	{
		ulChar = *pcStr++;
		if((ulChar < ' ') || (ulChar > '~'))
		{
			if(ulChar!='\n')	continue;
			else ulChar=0x7f;
		}
		// Convert the character into an index into the keyboard usage code table
		ulChar -= ' ';
		
		// Send the key press message.
		g_eKeyboardState = STATE_SENDING;
		if(USBDHIDKeyboardKeyStateChange((void *)&g_sKeyboardDevice,
			g_ppcKeyUsageCodes[ulChar][0], g_ppcKeyUsageCodes[ulChar][1], true) != KEYB_SUCCESS)
		{
			return;
		}
		// Wait until the key press message has been sent.
		if(!WaitForSendIdle(MAX_SEND_DELAY))
		{
			g_bConnected = 0;
			return;
		}
		// Send the key release message.
		g_eKeyboardState = STATE_SENDING;
		if(USBDHIDKeyboardKeyStateChange((void *)&g_sKeyboardDevice,
			g_ppcKeyUsageCodes[ulChar][0], g_ppcKeyUsageCodes[ulChar][1], false) != KEYB_SUCCESS)
		{
			return;
		}
		// Wait until the key release message has been sent.
		if(!WaitForSendIdle(MAX_SEND_DELAY))
		{
			g_bConnected = 0;
			return;
		}
	}
}

tBoolean WaitForSendIdle(unsigned long ulTimeoutTicks)
{
	unsigned long ulMillis = millis();	
	while((unsigned long)(millis() - ulMillis) < ulTimeoutTicks)
	{
		// Is the keyboard is idle, return immediately.
		if(g_eKeyboardState == STATE_IDLE)
			return true;
	}
	// If we get here, we timed out so return a bad return code to let the caller know.
	return(false);
}
