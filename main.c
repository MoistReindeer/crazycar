#include <msp430.h> 
#include <HAL/hal_general.h>
#include <HAL/hal_gpio.h>
#include <HAL/hal_pmm.h>
#include <DL/driver_general.h>
#include "HAL/hal_usciB1.h"
#include <DL/driver_lcd.h>
#include <DL/driver_rpm.h>
#include <HAL/hal_adc12.h>

extern ButtonCom BUTTONCOM;
extern USCIB1_SPICom SPICom;
extern unsigned int RPM_DISTANCE;
extern ADC12Com ADC12Data;

/**
 * main.c
 */
void main(void)
{
    HAL_Init();
    HAL_USCIB1_Init();
    Driver_Init();

    // Driver_LCD_Test();

	while (1)
	{
	    if (ADC12Data.Status.B.ADCrdy == 1) {
	        Driver_LCD_WriteUInt(ADC12Data.ADCBuffer[0], 0, 0);
            Driver_LCD_WriteUInt(ADC12Data.ADCBuffer[1], 1, 0);
            Driver_LCD_WriteUInt(ADC12Data.ADCBuffer[2], 2, 0);
            Driver_LCD_WriteUInt(ADC12Data.ADCBuffer[3], 3, 0);
            ADC12Data.Status.B.ADCrdy = 0;
	    }
	    Driver_LCD_WriteUInt((int)(RPM_DISTANCE/0.5), 4, 0);
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
