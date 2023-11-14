#include <msp430.h>
#include "HAL/hal_timerA1.h"
#include "HAL/hal_gpio.h"

void HAL_TimerA1_Init(void) {
    TA1CTL |= TASSEL_2;
    TA1CTL |= MC_1;
    TA1CCTL1 |= CCIE;
    TA1CCTL2 |= CCIE;
    TA1CCTL1 |= OUTMOD_7;
    TA1CCTL2 |= OUTMOD_7;

    TA1CCR0 = 41666;
    TA1CCR1 = 0;
    TA1CCR2 = 0;
    // TA1CCR0 wert für PWM = 41666
}
