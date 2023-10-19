#include <msp430.h>
#include "hal/hal_timerB0.h"

#define T_FREQ 2

void HAL_TimerB0_Init(void) {
    TB0CTL |= TBSSEL_2; //Setzt SMCLK als clock source
    TB0CTL |= MC_1; //Setzt den Timer in den UP-Mode
    TB0CTL |= ID_3; //Setzt den Input Divider zu /8
    TB0CCTL0 |= CCIE; //Enabled den Capture Compare Interrupt
    TB0EX0 |= TBIDEX_4; //Setzt den zweiten Divider zu /5
    TB0CCR0 = SMCLK_FREQU/T_FREQ/8/5; //Berechnet den Wert, der ins Capture Compare Register kommt
}
