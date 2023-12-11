#include <msp430.h>
#include <HAL/hal_adc12.h>

extern ADC12Com ADC12Data;

void HAL_ADC_Init(void) {
    REFCTL0 |= REFON;               // Enables reference voltage
    REFCTL0 |= REFOUT;              // Makes reference voltage available externally
    REFCTL0 |= REFVSEL_2;           // Sets the reference voltage to 2.5V

    ADC12CTL0 &= ~ADC12ENC;         // ADC deaktivieren, um veränderungen vorzunehmen
    ADC12CTL0 |= ADC12REFON;        // Enable the reference generator
    ADC12CTL0 |= ADC12REF2_5V;      // Set the reference voltage to 2.5
    ADC12CTL0 |= ADC12SHT0_12;      // Sets the sample-and-hold time to 1024 cycles
    ADC12CTL0 |= ADC12MSC;          // Further sample-and-conversion trigger automatically

    ADC12CTL1 |= ADC12SSEL_3;       // Sets the clock source to Submaster
    ADC12CTL1 |= ADC12SHS_3;        // Set sample trigger source to TimerB0 CCR1
    ADC12CTL1 |= ADC12CONSEQ_3;     // Selects sequence of channel mode
    ADC12CTL1 |= ADC12DIV_4;        // SMCLK divider /5
    ADC12CTL1 |= ADC12SHP;          // Sets SAMPCON to source from sampling timer

    ADC12CTL2 |= ADC12RES_2;        // Sets the resolution to 12 Bit

    ADC12MCTL3 |= ADC12EOS;         // Selects the last conversion
    ADC12MCTL0 |= ADC12SREF_1;      // Sets voltage reference to VREF
    ADC12MCTL1 |= ADC12SREF_1;
    ADC12MCTL2 |= ADC12SREF_1;
    ADC12MCTL3 |= ADC12SREF_1;

    ADC12MCTL0 |= ADC12INCH_0;      // Selects the input channel
    ADC12MCTL1 |= ADC12INCH_1;
    ADC12MCTL2 |= ADC12INCH_2;
    ADC12MCTL3 |= ADC12INCH_3;

    ADC12IE |= ADC12IE0;            // Enables interrupt for interrupt 0
    ADC12IE |= ADC12IE1;
    ADC12IE |= ADC12IE2;
    ADC12IE |= ADC12IE3;

    ADC12CTL0 |= ADC12ON;           // ADC on
    ADC12CTL0 |= ADC12ENC;          // ADC aktivieren
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC_ISR(void) {
    switch (ADC12IV) {
    case ADC12IV_ADC12IFG0:
        ADC12Data.ADCBuffer[0] = ADC12MEM0;
        ADC12Data.Status.B.ADCrdy = 1;
        break;
    case ADC12IV_ADC12IFG1:
        ADC12Data.ADCBuffer[1] = ADC12MEM1;
        ADC12Data.Status.B.ADCrdy = 1;
        break;
    case ADC12IV_ADC12IFG2:
        ADC12Data.ADCBuffer[2] = ADC12MEM2;
        ADC12Data.Status.B.ADCrdy = 1;
        break;
    case ADC12IV_ADC12IFG3:
        ADC12Data.ADCBuffer[3] = ADC12MEM3;
        ADC12Data.Status.B.ADCrdy = 1;
        break;
    }
}
