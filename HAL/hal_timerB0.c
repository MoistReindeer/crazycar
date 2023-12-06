#include <msp430.h>
#include <HAL/hal_ucs.h>
#include <HAL/hal_timerB0.h>

void HAL_TimerB0_Init(void) {
    TB0CTL |= TBSSEL__SMCLK;    //Setzt SMCLK als clock source
    TB0CTL |= MC_1;             //Setzt den Timer in den UP-Mode
    TB0CCTL0 |= CCIE;           //Enabled den Capture Compare Interrupt
    TB0CCTL1 |= OUTMOD_6;       // Einzelner Puls
    TB0CCR0 = 20833;            // Wert für eine Periodendauer von 120Hz
    TB0CCR1 = 1;                // Setzt für einen Puls als trigger
}
