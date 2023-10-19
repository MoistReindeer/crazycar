#include <msp430.h>
#include <HAL/hal_ucs.h>

void HAL_UCS_Init(void) {
    UCSCTL6 &= ~XT2OFF;
    UCSCTL3 |= SELREF_2;
    UCSCTL4 |= SELA_2;

    while (SFRIFG1 & OFIFG){
        UCSCTL7 &= ~(XT2OFFG + DCOFFG + XT1HFOFFG + XT1LFOFFG);
        SFRIFG1 &= ~OFIFG;
    }

    UCSCTL6 |= XT2DRIVE_2;
    UCSCTL4 |= SELS_5;
    UCSCTL4 |= SELM_5;
    UCSCTL5 |= DIVS_3;
}
