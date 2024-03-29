#include "HAL/hal_gpio.h"
#include <msp430.h>
#include <AL/al_control.h>

extern ButtonCom BUTTONCOM;
extern DriveStatus_t DriveStatus;

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
    P1DIR &= ~STOP_BUTTON;
    P1OUT |= STOP_BUTTON;
    P1IE |= STOP_BUTTON;
    P1IES |= STOP_BUTTON;

    // Start Button
    P1DIR &= ~START_BUTTON;
    P1OUT |= START_BUTTON;
    P1IE |= START_BUTTON;
    P1IES |= START_BUTTON;

    // Konfigurieren der Clock, XT2-Pins
    P7SEL |= XT2IN;
    P7SEL |= XT2OUT;
    UCSCTL6 &= ~XT2BYPASS;

    // SMCLK-Output Messung
    P3DIR |= SMCLK;
    P3SEL |= SMCLK;

    // Steering & Throttle
    P3DIR |= THROTTLE;
    P3DIR |= STEERING;
    P3SEL |= THROTTLE;
    P3SEL |= STEERING;

    // RPM Sensor
    P1SEL |= RPM_SENSOR;
    P1DIR &= ~RPM_SENSOR;
    P1DIR &= ~RPM_SENSOR_DIR;

    // SPI Configuration
    P8DIR |= LCD_BL; // LCD Backlight
    P8DIR |= LCD_SPI_CS;
    P8OUT |= LCD_SPI_CS;
    P8DIR |= LCD_SPI_CLK;
    P8SEL |= LCD_SPI_CLK;
    P8DIR |= LCD_SPI_MOSI;
    P8SEL |= LCD_SPI_MOSI;
    P8DIR |= LCD_SPI_MISO;
    P8SEL |= LCD_SPI_MISO;
    P8DIR |= LCD_DATACMD; // IO-Port als Ausgang
    P9DIR |= LCD_RESET;

    // ADC Configuration
    P6SEL |= DISTANCE_RIGHT;
    P6SEL |= DISTANCE_LEFT;
    P6SEL |= DISTANCE_FRONT;
    P6SEL |= VBAT_MEASURE;

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
    DriveStatus.requested = 1;
}
