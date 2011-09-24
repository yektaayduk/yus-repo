/*******************************
 * project: STM32 LED Blinker
 *    file: main.c
 *******************************/
 
/* memory map */
#define FLASH_BASE      0x08000000
#define SRAM_BASE       0x20000000
#define PERIPH_BASE     0x40000000
#define SRAM_SIZE       20*1024     // 20K RAM

/* RCC peripheral */
#define RCC_BASE        (PERIPH_BASE + 0x21000) // see "Reference Manual"
#define RCC_APB2ENR     (*(volatile unsigned long*)(RCC_BASE + 0x18))

/* GPIOB peripheral */
#define GPIOB_BASE      (PERIPH_BASE + 0x10C00) // see "Reference Manual"
#define GPIOB_CRL       (*(volatile unsigned long*)(GPIOB_BASE + 0x00))
#define GPIOB_CRH       (*(volatile unsigned long*)(GPIOB_BASE + 0x04))
#define GPIOB_BSRR      (*(volatile unsigned long*)(GPIOB_BASE + 0x10))
#define GPIOB_BRR       (*(volatile unsigned long*)(GPIOB_BASE + 0x14))

/* led connected to a gpio pin */
#define LED_PIN      9          // pin 9
#define OUTPUT_MODE (0x10|0x03) // push-pull + 50MHz

/* user functions */
int main(void);
void delay(unsigned long count);

/* vector table */
unsigned int *vector_table[] __attribute__((section(".vector_table"))) =
{
    (unsigned int *)(SRAM_BASE+SRAM_SIZE),   // end of SRAM
    (unsigned int *) main                    // entry point
};

int main()
{
    /* enable clock on GPIOB peripheral */
    RCC_APB2ENR = 0x08;
    
    /* set pin output mode */
    //GPIOB_CRL |= OUTPUT_MODE << ((LED_PIN) << 2); // if pins 0 to 7
    GPIOB_CRH |= OUTPUT_MODE << ((LED_PIN-8) << 2); // if pins 8 to 15

    while(1)
    {
        GPIOB_BSRR = 1<<LED_PIN;  // set pin high
        delay(200000);
        GPIOB_BRR  = 1<<LED_PIN;  // set pin low
        delay(200000);
    }
}

void delay(unsigned long count)
{
    while(count--);
}

