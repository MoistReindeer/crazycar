#include <msp430.h>

void HAL_ADC_Init(void) {
    REFCTL0 |= REFON;               // Enables reference voltage
    REFCTL0 |= REFOUT;              // Makes reference voltage available externally
    REFCTL0 |= REFVSEL_2;           // Sets the reference voltage to 2.5V

    ADC12CTL0 &= ~ADC12ENC;         // ADC deaktivieren, um veränderungen vorzunehmen
    ADC12CTL0 |= ADC12REFON;        // Enable the reference generator
    ADC12CTL0 |= ADC12REF2_5V;      // Set the reference voltage to 2.5
    ADC12CTL0 |= ADC12SHT0_12;      // Sets the sample-and-hold time to 1024 cycles

    ADC12CTL1 |= ADC12SSEL_3;       // Sets the clock source to Submaster
    ADC12CTL1 |= ADC12SHS_3;        // Set sample trigger source to TimerB0 CCR1

    ADC12CTL2 |= ADC12RES_2;

    ADC12CTL0 |= ADC12ENC;         // ADC aktivieren
}
