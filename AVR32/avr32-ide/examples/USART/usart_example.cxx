#include <avr32.h>

#define EXAMPLE_USART                 (&AVR32_USART3)
#define EXAMPLE_USART_RX_PIN          AVR32_USART3_RXD_0_0_PIN
#define EXAMPLE_USART_RX_FUNCTION     AVR32_USART3_RXD_0_0_FUNCTION
#define EXAMPLE_USART_TX_PIN          AVR32_USART3_TXD_0_0_PIN
#define EXAMPLE_USART_TX_FUNCTION     AVR32_USART3_TXD_0_0_FUNCTION
#define EXAMPLE_USART_CLOCK_MASK      AVR32_USART3_CLK_PBA

#define EXAMPLE_PDCA_CLOCK_HSB        AVR32_PDCA_CLK_HSB
#define EXAMPLE_PDCA_CLOCK_PB         AVR32_PDCA_CLK_PBA

int main(void)
{
	
	static const gpio_map_t USART_GPIO_MAP =
	{
		{EXAMPLE_USART_RX_PIN, EXAMPLE_USART_RX_FUNCTION},
		{EXAMPLE_USART_TX_PIN, EXAMPLE_USART_TX_FUNCTION}
	};
	
	// USART options.
	static const usart_options_t USART_OPTIONS =
	{
		115200,
		8,
		USART_NO_PARITY,
		USART_1_STOPBIT,
		USART_NORMAL_CHMODE
	};
	
	// Assign GPIO to USART.
	gpio_enable_module(USART_GPIO_MAP,
			sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));
	
	// Initialize USART in RS232 mode.
	usart_init_rs232(EXAMPLE_USART, &USART_OPTIONS, TARGET_PBACLK_FREQ_HZ);
	
	// Hello world!
	usart_write_line(EXAMPLE_USART, "This is the AVR UC3L MCU saying hello world!\r\n");

	// Press enter to continue.
	while (usart_get_echo_line(EXAMPLE_USART) == USART_FAILURE);  // Get and echo characters until end of line.

	usart_write_line(EXAMPLE_USART, "Thanks!\r\n");

	//*** Sleep mode
	// This program won't be doing anything else from now on, so it might as well
	// sleep.
	// Modules communicating with external circuits should normally be disabled
	// before entering a sleep mode that will stop the module operation.
	// Make sure the USART dumps the last message completely before turning it off.
	while(!usart_tx_empty(EXAMPLE_USART));
	pcl_disable_module(EXAMPLE_USART_CLOCK_MASK);

	// Since we're going into a sleep mode deeper than IDLE, all HSB masters must
	// be stopped before entering the sleep mode.
	pcl_disable_module(EXAMPLE_PDCA_CLOCK_HSB);
	pcl_disable_module(EXAMPLE_PDCA_CLOCK_PB);

	// If there is a chance that any PB write operations are incomplete, the CPU
	// should perform a read operation from any register on the PB bus before
	// executing the sleep instruction.
	AVR32_INTC.ipr[0];  // Dummy read

	// Go to STATIC sleep mode.
	SLEEP(AVR32_PM_SMODE_STATIC);

	while (true);
}
