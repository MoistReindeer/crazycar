#include <msp430.h>
#include "HAL/hal_usciB1.h"
#include "HAL/hal_gpio.h"

extern USCIB1_SPICom SPICom;

void HAL_USCIB1_Init(void) {
    UCB1CTL1 |= UCSWRST;
    UCB1CTL0 |= UCCKPL | UCMST | UCMSB;
    UCB1CTL0 |= UCMODE_0;
    UCB1CTL1 |= UCSSEL__SMCLK;
    UCB1BR0 = 25;
    UCB1BR1 = 0x00;
    UCB1CTL1 &= ~UCSWRST;
    UCB1IE |= UCRXIE;

    SPICom.TxData.len = 0;
    SPICom.TxData.cnt = 0;
    SPICom.Status.TxSuc = 1;
    SPICom.RxData.len = 0;
}

void HAL_USCIB1_Transmit(unsigned char len) {
    while(SPICom.Status.TxSuc == 0);

    SPICom.TxData.len = len;
    SPICom.TxData.cnt = 0;
    SPICom.Status.TxSuc = 0;
    SPICom.RxData.len = 0;

    UCB1TXBUF = SPICom.TxData.Data[0];
    LCD_CS_LOW;
}

#pragma vector = USCI_B1_VECTOR
__interrupt void rx_ISR(void) {
    switch(UCB1IFG) {
        case UCRXIFG:
            SPICom.RxData.Data[SPICom.TxData.cnt++] = UCB1RXBUF;
            if(SPICom.TxData.cnt < SPICom.TxData.len) {
                LCD_CS_LOW;
                UCB1TXBUF = SPICom.TxData.Data[SPICom.TxData.cnt];
            } else {
                LCD_CS_HIGH;
                SPICom.Status.TxSuc = 1;
            }
            break;
        default:
            UCB1IFG &= ~UCTXIFG;
            break;
    }
}
