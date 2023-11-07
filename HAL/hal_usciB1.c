#include <msp430.h>
#include "HAL/hal_usciB1.h"
#include "HAL/hal_gpio.h"

extern USCIB1_SPICom SPICom;

void HAL_USCIB1_Init(void) {
    UCB1CTL1 |= UCSWRST;    // Enables the software reset
    UCB1CTL0 |= UCMST;      // Sets the master mode
    UCB1BR0 = 25;         // Divides by 25
    UCB1BR1 = 0x00;
    UCB1CTL0 &= ~UC7BIT;    // Sets data to 8-bit length
    UCB1CTL0 |= UCMSB;      // Sets the MSB-first
    UCB1CTL0 &= ~UCCKPH;    // Sets the clock phase to change on first UCLK
    UCB1CTL0 |= UCCKPL;     // Sets the clock polarity to inactive high
    UCB1CTL1 &= ~UCSWRST;   // Disables the software reset

    UCB1IE |= UCRXIE;       // Enable the receive interrupt
    UCB1IE |= UCTXIE;       // Enable the transmit interrupt
}

void HAL_USCIB1_Transmit(void) {
    SPICom.Status.TxSuc = 0;
    P8OUT &= ~LCD_SPI_CS;

    //while(UCB1STAT & UCBUSY);

    UCB1TXBUF = SPICom.TxData.Data[0];
    SPICom.TxData.cnt += 1;
}

#pragma vector = USCI_B1_VECTOR
__interrupt void rx_ISR(void) {
    if (UCB1IFG & UCTXIFG) {
        UCB1TXBUF = SPICom.TxData.Data[1];
        SPICom.Status.TxSuc = 1;
    }
}
