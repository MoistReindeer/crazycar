#include "HAL/hal_gpio.h"
#include <msp430.h>

extern ButtonCom BUTTONCOM;

void HAL_GPIO_Init(void) {
    // ### Port 1 Default ###
    P1DIR = 0x00;
    P1REN = 0xFF;
    P1OUT = 0x00;

    // ### Port 2 Default ###
    P2DIR = 0x00;
    P2REN = 0xFF;
    P2OUT = 0x00;

    // ### Port 3 Default ###
    P3DIR = 0x00;
    P3REN = 0xFF;
    P3OUT = 0x00;

    // ### Port 4 Default ###
    P4DIR = 0x00;
    P4REN = 0xFF;
    P4OUT = 0x00;

    // ### Port 5 Default ###
    P5DIR = 0x00;
    P5REN = 0xFF;
    P5OUT = 0x00;

    // ### Port 6 Default ###
    P6DIR = 0x00;
    P6REN = 0xFF;
    P6OUT = 0x00;

    // ### Port 7 Default ###
    P7DIR = 0x00;
    P7REN = 0xFF;
    P7OUT = 0x00;

    // ### Port 8 Default ###
    P8DIR = 0x00;
    P8REN = 0xFF;
    P8OUT = 0x00;

    // ### Port 9 Default ###
    P9DIR = 0x00;
    P9REN = 0xFF;
    P9OUT = 0x00;

    // ### Custom Definitions ###

    // Stop Button
    P1DIR &= ~BIT7; // TODO: makros statt BIT definition verwenden
    P1OUT |= BIT7;
    P1IE |= BIT7;
    P1IES |= BIT7;

    // Start Button
    P1DIR &= ~BIT6;
    P1OUT |= BIT6;
    P1IE |= BIT6;
    P1IES |= BIT6;

    // Konfigurieren der Clock, XT2-Pins
    P7SEL |= BIT2;
    P7SEL |= BIT3;
    UCSCTL6 &= ~XT2BYPASS;

    // SMCLK-Output Messung
    P3DIR |= BIT4;
    P3SEL |= BIT4;

    // Steering & Throttle
    P3DIR |= THROTTLE;
    P3DIR |= STEERING;
    P3SEL |= THROTTLE;
    P3SEL |= STEERING;

    // SPI Configuration
    P8DIR |= LCD_BL; // LCD Backlight
    P8DIR |= LCD_SPI_CS;
    P8DIR |= LCD_SPI_CLK;
    P8SEL |= LCD_SPI_CLK;
    P8DIR |= LCD_SPI_MOSI;
    P8SEL |= LCD_SPI_MOSI;
    P8DIR &= ~LCD_SPI_MISO;
    P8SEL |= LCD_SPI_MISO;
    P8DIR |= LCD_DATACMD; // IO-Port als Ausgang

    __enable_interrupt();
}

#pragma vector=PORT1_VECTOR
__interrupt void P1_ISR(void) {
    switch (P1IFG) {
    case START_BUTTON: // TODO - Buttons mit 'P1IFG &' maskieren
            BUTTONCOM.active = 1;
            BUTTONCOM.button = 0;
            P1IFG &= ~START_BUTTON;
            break;
    case STOP_BUTTON:
            BUTTONCOM.active = 1;
            BUTTONCOM.button = 1;
            P1IFG &= ~STOP_BUTTON;
            break;
    default:
            P1IFG = 0x00;
    }
}

#pragma vector = TIMERB0_VECTOR
__interrupt void TimerB0(void) {

}
