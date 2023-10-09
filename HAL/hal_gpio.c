#include "HAL/hal_gpio.h"
#include <msp430.h>

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
    // LCD Backlight
    P8DIR |= LCD_BL;
}
