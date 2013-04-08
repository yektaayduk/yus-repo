//*****************************************************************************
//
// startup.c - Startup code for use with GNU tools.
//
// Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
//*****************************************************************************

#include "inc/hw_nvic.h"
#include "inc/hw_types.h"

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);

//*****************************************************************************
//
// overridable interrupt handlers
//
//*****************************************************************************
__attribute__((weak)) void MPUFaultHandler(void) { while(1) {} }
__attribute__((weak)) void SVCIntHandler(void) { while(1) {} }
__attribute__((weak)) void PendSVIntHandler(void) { while(1) {} }
__attribute__((weak)) void SysTickIntHandler(void) { while(1) {} }
__attribute__((weak)) void GPIOAIntHandler(void) { while(1) {} }
__attribute__((weak)) void GPIOBIntHandler(void) { while(1) {} }
__attribute__((weak)) void GPIOCIntHandler(void) { while(1) {} }
__attribute__((weak)) void GPIODIntHandler(void) { while(1) {} }
__attribute__((weak)) void GPIOEIntHandler(void) { while(1) {} }
__attribute__((weak)) void UART0IntHandler(void) { while(1) {} }
__attribute__((weak)) void UART1IntHandler(void) { while(1) {} }
__attribute__((weak)) void SSI0IntHandler(void) { while(1) {} }
__attribute__((weak)) void I2C0IntHandler(void) { while(1) {} }
//__attribute__((weak)) void PWM0FaultIntHandler(void) { while(1) {} }
//__attribute__((weak)) void PWM0Gen0IntHandler(void) { while(1) {} }
//__attribute__((weak)) void PWM0Gen1IntHandler(void) { while(1) {} }
//__attribute__((weak)) void PWM0Gen2IntHandler(void) { while(1) {} }
//__attribute__((weak)) void QuadratureEncoder0IntHandler(void) { while(1) {} }
__attribute__((weak)) void ADC0Sequence0IntHandler(void) { while(1) {} }
__attribute__((weak)) void ADC0Sequence1IntHandler(void) { while(1) {} }
__attribute__((weak)) void ADC0Sequence2IntHandler(void) { while(1) {} }
__attribute__((weak)) void ADC0Sequence3IntHandler(void) { while(1) {} }
__attribute__((weak)) void WatchdogTimerIntHandler(void) { while(1) {} }
__attribute__((weak)) void Timer0AIntHandler(void) { while(1) {} }
__attribute__((weak)) void Timer0BIntHandler(void) { while(1) {} }
__attribute__((weak)) void Timer1AIntHandler(void) { while(1) {} }
__attribute__((weak)) void Timer1BIntHandler(void) { while(1) {} }
__attribute__((weak)) void Timer2AIntHandler(void) { while(1) {} }
__attribute__((weak)) void Timer2BIntHandler(void) { while(1) {} }
__attribute__((weak)) void AnalogComparator0IntHandler(void) { while(1) {} }
__attribute__((weak)) void AnalogComparator1IntHandler(void) { while(1) {} }
//__attribute__((weak)) void AnalogComparator2IntHandler(void) { while(1) {} }
//__attribute__((weak)) void SystemControlIntHandler(void) { while(1) {} }
//__attribute__((weak)) void FLASHControlIntHandler(void) { while(1) {} }
__attribute__((weak)) void GPIOFIntHandler(void) { while(1) {} }
//__attribute__((weak)) void GPIOGIntHandler(void) { while(1) {} }
//__attribute__((weak)) void GPIOHIntHandler(void) { while(1) {} }
__attribute__((weak)) void UART2IntHandler(void) { while(1) {} }
__attribute__((weak)) void SSI1IntHandler(void) { while(1) {} }
__attribute__((weak)) void Timer3AIntHandler(void) { while(1) {} }
__attribute__((weak)) void Timer3BIntHandler(void) { while(1) {} }
__attribute__((weak)) void I2C1IntHandler(void) { while(1) {} }
//__attribute__((weak)) void QuadratureEncoder1IntHandler(void) { while(1) {} }
__attribute__((weak)) void CAN0IntHandler(void) { while(1) {} }
//__attribute__((weak)) void CAN1IntHandler(void) { while(1) {} }
//__attribute__((weak)) void CAN2IntHandler(void) { while(1) {} }
//__attribute__((weak)) void EthernetIntHandler(void) { while(1) {} }
__attribute__((weak)) void HibernateIntHandler(void) { while(1) {} }
__attribute__((weak)) void USB0DeviceIntHandler(void) { while(1) {} }
//__attribute__((weak)) void PWM0Gen3IntHandler(void) { while(1) {} }
__attribute__((weak)) void uDMASoftTransferIntHandler(void) { while(1) {} }
__attribute__((weak)) void uDMAErrorIntHandler(void) { while(1) {} }
__attribute__((weak)) void ADC1Sequence0IntHandler(void) { while(1) {} }
__attribute__((weak)) void ADC1Sequence1IntHandler(void) { while(1) {} }
__attribute__((weak)) void ADC1Sequence2IntHandler(void) { while(1) {} }
__attribute__((weak)) void ADC1Sequence3IntHandler(void) { while(1) {} }
//__attribute__((weak)) void I2S0IntHandler(void) { while(1) {} }
//__attribute__((weak)) void ExtBusInterface0IntHandler(void) { while(1) {} }
//__attribute__((weak)) void GPIOJIntHandler(void) { while(1) {} }
//__attribute__((weak)) void GPIOKIntHandler(void) { while(1) {} }
//__attribute__((weak)) void GPIOLIntHandler(void) { while(1) {} }
__attribute__((weak)) void SSI2IntHandler(void) { while(1) {} }
__attribute__((weak)) void SSI3IntHandler(void) { while(1) {} }
__attribute__((weak)) void UART3IntHandler(void) { while(1) {} }
__attribute__((weak)) void UART4IntHandler(void) { while(1) {} }
__attribute__((weak)) void UART5IntHandler(void) { while(1) {} }
__attribute__((weak)) void UART6IntHandler(void) { while(1) {} }
__attribute__((weak)) void UART7IntHandler(void) { while(1) {} }
__attribute__((weak)) void I2C2IntHandler(void) { while(1) {} }
__attribute__((weak)) void I2C3IntHandler(void) { while(1) {} }
__attribute__((weak)) void Timer4AIntHandler(void) { while(1) {} }
__attribute__((weak)) void Timer4BIntHandler(void) { while(1) {} }
__attribute__((weak)) void Timer5AIntHandler(void) { while(1) {} }
__attribute__((weak)) void Timer5BIntHandler(void) { while(1) {} }
__attribute__((weak)) void WTimer0AIntHandler(void) { while(1) {} }
__attribute__((weak)) void WTimer0BIntHandler(void) { while(1) {} }
__attribute__((weak)) void WTimer1AIntHandler(void) { while(1) {} }
__attribute__((weak)) void WTimer1BIntHandler(void) { while(1) {} }
__attribute__((weak)) void WTimer2AIntHandler(void) { while(1) {} }
__attribute__((weak)) void WTimer2BIntHandler(void) { while(1) {} }
__attribute__((weak)) void WTimer3AIntHandler(void) { while(1) {} }
__attribute__((weak)) void WTimer3BIntHandler(void) { while(1) {} }
//__attribute__((weak)) void WTimer4AIntHandler(void) { while(1) {} }
//__attribute__((weak)) void WTimer4BIntHandler(void) { while(1) {} }
__attribute__((weak)) void WTimer5AIntHandler(void) { while(1) {} }
__attribute__((weak)) void WTimer5BIntHandler(void) { while(1) {} }
__attribute__((weak)) void FPUIntHandler(void) { while(1) {} }
//__attribute__((weak)) void PECI0IntHandler(void) { while(1) {} }
//__attribute__((weak)) void LPC0IntHandler(void) { while(1) {} }
//__attribute__((weak)) void I2C4IntHandler(void) { while(1) {} }
//__attribute__((weak)) void I2C5IntHandler(void) { while(1) {} }
//__attribute__((weak)) void GPIOMIntHandler(void) { while(1) {} }
//__attribute__((weak)) void GPIONIntHandler(void) { while(1) {} }
//__attribute__((weak)) void QuadratureEncoder2IntHandler(void) { while(1) {} }
//__attribute__((weak)) void Fan0IntHandler(void) { while(1) {} }
//__attribute__((weak)) void GPIOPIntHandler(void) { while(1) {} }
//__attribute__((weak)) void GPIOQIntHandler(void) { while(1) {} }
//__attribute__((weak)) void GPIORIntHandler(void) { while(1) {} }
//__attribute__((weak)) void GPIOSIntHandler(void) { while(1) {} }
//__attribute__((weak)) void PWM1Gen0IntHandler(void) { while(1) {} }
//__attribute__((weak)) void PWM1Gen1IntHandler(void) { while(1) {} }
//__attribute__((weak)) void PWM1Gen2IntHandler(void) { while(1) {} }
//__attribute__((weak)) void PWM1Gen3IntHandler(void) { while(1) {} }
//__attribute__((weak)) void PWM1FaultIntHandler(void) { while(1) {} }

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);


//*****************************************************************************
//
// System stack start determined by ldscript, normally highest ram address
//
//*****************************************************************************
extern unsigned _estack;

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
    (void *)&_estack,                       // The initial stack pointer
    ResetISR,                               // The reset handler
    NmiSR,                                  // The NMI handler
    FaultISR,                               // The hard fault handler
    MPUFaultHandler,                        // The MPU fault handler
    IntDefaultHandler,                      // The bus fault handler
    IntDefaultHandler,                      // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    SVCIntHandler,                          // SVCall handler
    IntDefaultHandler,                      // Debug monitor handler
    0,                                      // Reserved
    PendSVIntHandler,                       // The PendSV handler
    SysTickIntHandler,                      // The SysTick handler
    GPIOAIntHandler,                        // GPIO Port A
    GPIOBIntHandler,                        // GPIO Port B
    GPIOCIntHandler,                        // GPIO Port C
    GPIODIntHandler,                        // GPIO Port D
    GPIOEIntHandler,                        // GPIO Port E
    UART0IntHandler,                        // UART0 Rx and Tx
    UART1IntHandler,                        // UART1 Rx and Tx
    SSI0IntHandler,                         // SSI0 Rx and Tx
    I2C0IntHandler,                         // I2C0 Master and Slave
    IntDefaultHandler,                      // PWM Fault
    IntDefaultHandler,                      // PWM Generator 0
    IntDefaultHandler,                      // PWM Generator 1
    IntDefaultHandler,                      // PWM Generator 2
    IntDefaultHandler,                      // Quadrature Encoder 0
    ADC0Sequence0IntHandler,                // ADC Sequence 0
    ADC0Sequence1IntHandler,                // ADC Sequence 1
    ADC0Sequence2IntHandler,                // ADC Sequence 2
    ADC0Sequence3IntHandler,                // ADC Sequence 3
    WatchdogTimerIntHandler,                // Watchdog timer
    Timer0AIntHandler,                      // Timer 0 subtimer A
    Timer0BIntHandler,                      // Timer 0 subtimer B
    Timer1AIntHandler,                      // Timer 1 subtimer A
    Timer1BIntHandler,                      // Timer 1 subtimer B
    Timer2AIntHandler,                      // Timer 2 subtimer A
    Timer2BIntHandler,                      // Timer 2 subtimer B
    AnalogComparator0IntHandler,            // Analog Comparator 0
    AnalogComparator1IntHandler,            // Analog Comparator 1
    IntDefaultHandler,                      // Analog Comparator 2
    IntDefaultHandler,                      // System Control (PLL, OSC, BO)
    IntDefaultHandler,                      // FLASH Control
    GPIOFIntHandler,                        // GPIO Port F
    IntDefaultHandler,                      // GPIO Port G
    IntDefaultHandler,                      // GPIO Port H
    UART2IntHandler,                        // UART2 Rx and Tx
    SSI1IntHandler,                         // SSI1 Rx and Tx
    Timer3AIntHandler,                      // Timer 3 subtimer A
    Timer3BIntHandler,                      // Timer 3 subtimer B
    I2C1IntHandler,                         // I2C1 Master and Slave
    IntDefaultHandler,                      // Quadrature Encoder 1
    CAN0IntHandler,                         // CAN0
    IntDefaultHandler,                      // CAN1
    IntDefaultHandler,                      // CAN2
    IntDefaultHandler,                      // Ethernet
    HibernateIntHandler,                    // Hibernate
    USB0DeviceIntHandler,                   // USB0
    IntDefaultHandler,                      // PWM Generator 3
    uDMASoftTransferIntHandler,             // uDMA Software Transfer
    uDMAErrorIntHandler,                    // uDMA Error
    ADC1Sequence0IntHandler,                // ADC1 Sequence 0
    ADC1Sequence1IntHandler,                // ADC1 Sequence 1
    ADC1Sequence2IntHandler,                // ADC1 Sequence 2
    ADC1Sequence3IntHandler,                // ADC1 Sequence 3
    IntDefaultHandler,                      // I2S0
    IntDefaultHandler,                      // External Bus Interface 0
    IntDefaultHandler,                      // GPIO Port J
    IntDefaultHandler,                      // GPIO Port K
    IntDefaultHandler,                      // GPIO Port L
    SSI2IntHandler,                         // SSI2 Rx and Tx
    SSI3IntHandler,                         // SSI3 Rx and Tx
    UART3IntHandler,                        // UART3 Rx and Tx
    UART4IntHandler,                        // UART4 Rx and Tx
    UART5IntHandler,                        // UART5 Rx and Tx
    UART6IntHandler,                        // UART6 Rx and Tx
    UART7IntHandler,                        // UART7 Rx and Tx
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    I2C2IntHandler,                         // I2C2 Master and Slave
    I2C3IntHandler,                         // I2C3 Master and Slave
    Timer4AIntHandler,                      // Timer 4 subtimer A
    Timer4BIntHandler,                      // Timer 4 subtimer B
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    Timer5AIntHandler,                      // Timer 5 subtimer A
    Timer5AIntHandler,                      // Timer 5 subtimer B
    WTimer0AIntHandler,                     // Wide Timer 0 subtimer A
    WTimer0BIntHandler,                     // Wide Timer 0 subtimer B
    WTimer1AIntHandler,                     // Wide Timer 1 subtimer A
    WTimer1BIntHandler,                     // Wide Timer 1 subtimer B
    WTimer2AIntHandler,                     // Wide Timer 2 subtimer A
    WTimer2BIntHandler,                     // Wide Timer 2 subtimer B
    WTimer3AIntHandler,                     // Wide Timer 3 subtimer A
    WTimer3BIntHandler,                     // Wide Timer 3 subtimer B
    IntDefaultHandler,                      // Wide Timer 4 subtimer A
    IntDefaultHandler,                      // Wide Timer 4 subtimer B
    WTimer5AIntHandler,                     // Wide Timer 5 subtimer A
    WTimer5BIntHandler,                     // Wide Timer 5 subtimer B
    FPUIntHandler,                          // FPU
    IntDefaultHandler,                      // PECI 0
    IntDefaultHandler,                      // LPC 0
    IntDefaultHandler,                      // I2C4 Master and Slave
    IntDefaultHandler,                      // I2C5 Master and Slave
    IntDefaultHandler,                      // GPIO Port M
    IntDefaultHandler,                      // GPIO Port N
    IntDefaultHandler,                      // Quadrature Encoder 2
    IntDefaultHandler,                      // Fan 0
    0,                                      // Reserved
    IntDefaultHandler,                      // GPIO Port P (Summary or P0)
    IntDefaultHandler,                      // GPIO Port P1
    IntDefaultHandler,                      // GPIO Port P2
    IntDefaultHandler,                      // GPIO Port P3
    IntDefaultHandler,                      // GPIO Port P4
    IntDefaultHandler,                      // GPIO Port P5
    IntDefaultHandler,                      // GPIO Port P6
    IntDefaultHandler,                      // GPIO Port P7
    IntDefaultHandler,                      // GPIO Port Q (Summary or Q0)
    IntDefaultHandler,                      // GPIO Port Q1
    IntDefaultHandler,                      // GPIO Port Q2
    IntDefaultHandler,                      // GPIO Port Q3
    IntDefaultHandler,                      // GPIO Port Q4
    IntDefaultHandler,                      // GPIO Port Q5
    IntDefaultHandler,                      // GPIO Port Q6
    IntDefaultHandler,                      // GPIO Port Q7
    IntDefaultHandler,                      // GPIO Port R
    IntDefaultHandler,                      // GPIO Port S
    IntDefaultHandler,                      // PWM 1 Generator 0
    IntDefaultHandler,                      // PWM 1 Generator 1
    IntDefaultHandler,                      // PWM 1 Generator 2
    IntDefaultHandler,                      // PWM 1 Generator 3
    IntDefaultHandler                       // PWM 1 Fault
};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;

//*****************************************************************************
//
// libc.a: constructors of static objects
//
//*****************************************************************************
extern void __libc_init_array(void);

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void
ResetISR(void)
{
    unsigned long *pulSrc, *pulDest;

    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.
    //
    __asm("    ldr     r0, =_bss\n"
          "    ldr     r1, =_ebss\n"
          "    mov     r2, #0\n"
          "    .thumb_func\n"
          "zero_loop:\n"
          "        cmp     r0, r1\n"
          "        it      lt\n"
          "        strlt   r2, [r0], #4\n"
          "        blt     zero_loop");
    
    //
    // call static constructors
    //
    __libc_init_array();

    //
    // Enable the floating-point unit.  This must be done here to handle the
    // case where main() uses floating-point and the function prologue saves
    // floating-point registers (which will fault if floating-point is not
    // enabled).  Any configuration of the floating-point unit using DriverLib
    // APIs must be done here prior to the floating-point unit being enabled.
    //
    // Note that this does not use DriverLib since it might not be included in
    // this project.
    //
    HWREG(NVIC_CPAC) = ((HWREG(NVIC_CPAC) &
                         ~(NVIC_CPAC_CP10_M | NVIC_CPAC_CP11_M)) |
                        NVIC_CPAC_CP10_FULL | NVIC_CPAC_CP11_FULL);

    //
    // Call the application's entry point.
    //
    main();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void
NmiSR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
FaultISR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
IntDefaultHandler(void)
{
    //
    // Go into an infinite loop.
    //
    while(1)
    {
    }
}
