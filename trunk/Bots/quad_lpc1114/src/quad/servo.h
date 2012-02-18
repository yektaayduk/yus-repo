

#ifndef __SERVO_H__
#define __SERVO_H__

// servo timings
#define SERVO_FREQUENCY          50    // in Hz
#define SERVO_ANGLE_RESOLUTION   1    // in degrees
#define SERVO_ANGLE_RANGE        180    // in degrees
#define SERVO_MIN                400 //1000 // in microseconds
#define SERVO_MAX                2700 //2000 // in microseconds
#define SERVO_TIME_RESOLUTION    (float)(SERVO_ANGLE_RESOLUTION*(SERVO_MAX-SERVO_MIN)/SERVO_ANGLE_RANGE)

// timer interrupt
#define SERVO_INTERRUPT_RATE     (uint32_t)((1E6/SERVO_TIME_RESOLUTION) + 0.5)

// servo counter
#define SERVO_MIN_COUNT          (uint16_t)(SERVO_MIN/SERVO_TIME_RESOLUTION)
#define SERVO_MAX_COUNT          (uint16_t)(SERVO_MAX/SERVO_TIME_RESOLUTION)
#define SERVO_RESET_COUNT        (uint16_t)(SERVO_INTERRUPT_RATE/SERVO_FREQUENCY)
#define ANGLE_TO_COUNTER(x)      (uint16_t)(SERVO_MIN_COUNT + x*(SERVO_MAX_COUNT-SERVO_MIN_COUNT)/SERVO_ANGLE_RANGE)

#define INITIAL_ANGLE            90
#define INITIAL_COUNT            ANGLE_TO_COUNTER(INITIAL_ANGLE)

//-------------------------------------------------------------
#define NSERVOS                  12
/* lpcxpresso lpc1114
    1_5--|21     48|--2_6
    1_8--|22     49|--2_7
    0_6--|23     50|--2_8
       --|24     51|--2_9
    3_0--|25     52|--2_10
    3_1--|26     53|--3_3
    3_2--|27     54|--GND
         -----------       */

/* servo  1 : PIO1_5  */
#define SERVO1_PORT     LPC_GPIO1
#define SERVO1_PIN      (1<<5)
#define SERVO1_HIGH()   SERVO1_PORT->MASKED_ACCESS[SERVO1_PIN] = SERVO1_PIN
#define SERVO1_LOW()    SERVO1_PORT->MASKED_ACCESS[SERVO1_PIN] = 0

/* servo  2 : PIO1_8  */
#define SERVO2_PORT     LPC_GPIO1
#define SERVO2_PIN      (1<<8)
#define SERVO2_HIGH()   SERVO2_PORT->MASKED_ACCESS[SERVO2_PIN] = SERVO2_PIN
#define SERVO2_LOW()    SERVO2_PORT->MASKED_ACCESS[SERVO2_PIN] = 0

/* servo  3 : PIO0_6  */
#define SERVO3_PORT     LPC_GPIO0
#define SERVO3_PIN      (1<<6)
#define SERVO3_HIGH()   SERVO3_PORT->MASKED_ACCESS[SERVO3_PIN] = SERVO3_PIN
#define SERVO3_LOW()    SERVO3_PORT->MASKED_ACCESS[SERVO3_PIN] = 0

/* servo  4 : PIO3_0  */
#define SERVO4_PORT     LPC_GPIO3
#define SERVO4_PIN      (1<<0)
#define SERVO4_HIGH()   SERVO4_PORT->MASKED_ACCESS[SERVO4_PIN] = SERVO4_PIN
#define SERVO4_LOW()    SERVO4_PORT->MASKED_ACCESS[SERVO4_PIN] = 0

/* servo  5 : PIO3_1  */
#define SERVO5_PORT     LPC_GPIO3
#define SERVO5_PIN      (1<<1)
#define SERVO5_HIGH()   SERVO5_PORT->MASKED_ACCESS[SERVO5_PIN] = SERVO5_PIN
#define SERVO5_LOW()    SERVO5_PORT->MASKED_ACCESS[SERVO5_PIN] = 0

/* servo  6 : PIO3_2  */
#define SERVO6_PORT     LPC_GPIO3
#define SERVO6_PIN      (1<<2)
#define SERVO6_HIGH()   SERVO6_PORT->MASKED_ACCESS[SERVO6_PIN] = SERVO6_PIN
#define SERVO6_LOW()    SERVO6_PORT->MASKED_ACCESS[SERVO6_PIN] = 0

/* servo  7 : PIO2_6  */
#define SERVO7_PORT     LPC_GPIO2
#define SERVO7_PIN      (1<<6)
#define SERVO7_HIGH()   SERVO7_PORT->MASKED_ACCESS[SERVO7_PIN] = SERVO7_PIN
#define SERVO7_LOW()    SERVO7_PORT->MASKED_ACCESS[SERVO7_PIN] = 0

/* servo  8 : PIO2_7  */
#define SERVO8_PORT     LPC_GPIO2
#define SERVO8_PIN      (1<<7)
#define SERVO8_HIGH()   SERVO8_PORT->MASKED_ACCESS[SERVO8_PIN] = SERVO8_PIN
#define SERVO8_LOW()    SERVO8_PORT->MASKED_ACCESS[SERVO8_PIN] = 0

/* servo  9 : PIO2_8  */
#define SERVO9_PORT     LPC_GPIO2
#define SERVO9_PIN      (1<<8)
#define SERVO9_HIGH()   SERVO9_PORT->MASKED_ACCESS[SERVO9_PIN] = SERVO9_PIN
#define SERVO9_LOW()    SERVO9_PORT->MASKED_ACCESS[SERVO9_PIN] = 0

/* servo 10 : PIO2_9  */
#define SERVO10_PORT    LPC_GPIO2
#define SERVO10_PIN     (1<<9)
#define SERVO10_HIGH()  SERVO10_PORT->MASKED_ACCESS[SERVO10_PIN] = SERVO10_PIN
#define SERVO10_LOW()   SERVO10_PORT->MASKED_ACCESS[SERVO10_PIN] = 0

/* servo 11 : PIO2_10 */
#define SERVO11_PORT    LPC_GPIO2
#define SERVO11_PIN     (1<<10)
#define SERVO11_HIGH()  SERVO11_PORT->MASKED_ACCESS[SERVO11_PIN] = SERVO11_PIN
#define SERVO11_LOW()   SERVO11_PORT->MASKED_ACCESS[SERVO11_PIN] = 0

/* servo 12 : PIO3_3  */
#define SERVO12_PORT    LPC_GPIO3
#define SERVO12_PIN     (1<<3)
#define SERVO12_HIGH()  SERVO12_PORT->MASKED_ACCESS[SERVO12_PIN] = SERVO12_PIN
#define SERVO12_LOW()   SERVO12_PORT->MASKED_ACCESS[SERVO12_PIN] = 0


//-------------------------------------------------------------
// servo pwm counter
extern volatile uint16_t servo_pwm[NSERVOS];

void servos_init(void);

#endif /* __SERVO_H__ */
