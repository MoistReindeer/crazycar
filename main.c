#include <msp430.h> 
#include <HAL/hal_general.h>
#include <HAL/hal_gpio.h>
#include <HAL/hal_pmm.h>
#include <DL/driver_general.h>
#include "HAL/hal_usciB1.h"
#include <DL/driver_lcd.h>
#include <DL/driver_rpm.h>
#include <HAL/hal_adc12.h>
#include <AL/al_conv.h>
#include <AL/al_control.h>

#define AVG 32

extern ButtonCom BUTTONCOM;
extern USCIB1_SPICom SPICom;
extern unsigned int RPM_DISTANCE;
extern ADC12Com ADC12Data;
extern ConversionData ConvertedData;

/**
 * main.c
 */
void main(void)
{
    HAL_Init();
    HAL_USCIB1_Init();
    Driver_Init();
    AL_Param_Init();

    // Driver_LCD_Test();

	while (1)
	{
	    if (ADC12Data.Status.B.ADCrdy == 1) {
	        Fetch_Distance();
	        AL_Fetch_Direction();
	        AL_Control_Steer();
            Driver_LCD_WriteUInt((ConvertedData.velocity_dd << 1), 4, 0); // division by 0.5 replaced with multiplication by 2
            Driver_LCD_WriteUInt(ConvertedData.Distance.front, 0, 0);
            Driver_LCD_WriteUInt(ConvertedData.Distance.left, 1, 0);
            Driver_LCD_WriteUInt(ConvertedData.Distance.right, 2, 0);
	    }
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
