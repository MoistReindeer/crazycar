#include <msp430.h>
#include "HAL/hal_timerA1.h"
#include "HAL/hal_gpio.h"

#define STEER_MIDDLE 3550
#define STEER_RIGHT 4300
#define STEER_LEFT 2900

void HAL_TimerA1_Init(void) {
    TA1CTL |= TASSEL_2;
    TA1CTL |= MC_1;
    TA1CCTL1 |= CCIE;
    TA1CCTL2 |= CCIE;
    TA1CCTL1 |= OUTMOD_7;
    TA1CCTL2 |= OUTMOD_7;
}
