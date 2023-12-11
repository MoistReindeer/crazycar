#include <HAL/hal_dma.h>
#include <HAL/hal_adc12.h>
#include <msp430.h>

extern ADC12Com ADC12Data;

void HAL_DMA_Init(void) {
    DMACTL0 |= DMA0TSEL_24;     // Selects the ADC12IFG as trigger for DMA0
    DMA0CTL |= DMADT_5;         // Selects repeated block transfer as transfer mode
    DMA0CTL |= DMADSTINCR_3;    // Increments dest address
    DMA0CTL |= DMASRCINCR_3;    // Increments src address
    DMA0CTL |= DMAIE;           // Enable DMA interrupt

    DMA0SA = &ADC12MEM0;
    DMA0DA = &ADC12Data.ADCBuffer[0];
    DMA0SZ = 4;

    DMA0CTL |= DMAEN;           // Enable DMA
    DMA0CTL |= DMAREQ;          // Request DMS
}

#pragma vector = DMA_VECTOR
__interrupt void DMA_ISR(void) {
    DMA0CTL &= ~DMAIFG;
    ADC12Data.Status.B.ADCrdy = 1;
}
