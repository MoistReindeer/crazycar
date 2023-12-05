#include <msp430.h>
#include <DL/driver_rpm.h>
#include <HAL/hal_gpio.h>
#include <HAL/hal_ucs.h>

extern int RPM_DISTANCE;
int rpm_cnt;

void RPM_Count_Init(void) { // Init the RPM_Counter @ Timer A0
    TA0CTL |= TASSEL__SMCLK;    // Set the input CLK to Submaster
    TA0CTL |= MC__UP;           // Set the timer to UP-Mode
    TA0CTL |= ID_2;             // Divide by 4

    TA0CCTL0 |= CCIE;

    // Timer 2
    TA0CCTL2 |= CAP;            // Enable capture mode
    TA0CCTL2 |= SCS;            // Synchronise to next clk signal
    TA0CCTL2 |= CM_1;           // Capture on rising edge
    TA0CCTL2 |= CCIS_0;         // Set capture compare input to CCI2A
    TA0CCTL2 |= CCIE;           // Enable interrupts for this control register

    TA0CCR0 = 62500;            // Measurement periode of 100ms
    rpm_cnt = 0;
}

#pragma vector=TIMER0_A1_VECTOR;
__interrupt void RPM_CNT (void) {
    rpm_cnt++;
    TA0CCTL2 &= ~CCIFG;
    TA0CCTL2 &= ~COV;
    TA0CTL &= ~TAIFG;
}

#pragma vector=TIMER0_A0_VECTOR;
__interrupt void RPM_CALC (void) {
    RPM_DISTANCE = rpm_cnt * 5;
    rpm_cnt=0;
    TA0CTL &= ~TAIFG;
}
