#include <msp430.h> 
#include <HAL/hal_general.h>
#include <HAL/hal_gpio.h>
#include <HAL/hal_pmm.h>
#include <DL/driver_general.h>
#include "HAL/hal_usciB1.h"
#include <DL/driver_lcd.h>
#include <DL/driver_rpm.h>

extern ButtonCom BUTTONCOM;
extern USCIB1_SPICom SPICom;
extern unsigned int RPM_DISTANCE;

/**
 * main.c
 */
void main(void)
{
    HAL_Init();
    HAL_USCIB1_Init();
    Driver_Init();

    //Driver_LCD_Test();

	while (1)
	{
	    Driver_LCD_WriteUInt((int)(RPM_DISTANCE/0.5), 0, 0);
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
