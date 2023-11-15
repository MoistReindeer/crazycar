#include <msp430.h> 
#include <HAL/hal_general.h>
#include <HAL/hal_gpio.h>
#include <HAL/hal_pmm.h>
#include <DL/driver_general.h>
#include "HAL/hal_usciB1.h"

extern ButtonCom BUTTONCOM;
extern USCIB1_SPICom SPICom;

/**
 * main.c
 */
void main(void)
{
    HAL_Init();
    HAL_USCIB1_Init();
    Driver_Init();
	
    SPICom.TxData.Data[0] = 0x0F;
    SPICom.TxData.Data[1] = 0xF0;

	while (1)
	{
	    HAL_USCIB1_Transmit(2);
	    if (BUTTONCOM.active == 1) {
	        if (BUTTONCOM.button == 0) {
	            LCD_BACKLIGHT_ON;
	        } else if (BUTTONCOM.button == 1){
	            LCD_BACKLIGHT_OFF;
	        }
	        BUTTONCOM.active = 0;
	    }
	}
}
