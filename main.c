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
extern DriveStatus_t DriveStatus;

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
    DriveStatus.refreshCount = 0;
    DriveStatus.start = 0;

	while (1)
	{
	    if (DriveStatus.requested == 1 && ADC12Data.Status.B.ADCrdy == 1) {
	        Fetch_Distance();
            AL_Fetch_Direction();
            //AL_Control_Steer();
            if (DriveStatus.refreshCount >= 120) {
                //Driver_LCD_WriteUInt((ConvertedData.velocity_dd << 1), 4, 0); // division by 0.5 replaced with multiplication by 2
                Driver_LCD_WriteText("f", 1, 0, 0);
                Driver_LCD_WriteUInt(ConvertedData.Distance.front, 0, 8);
                Driver_LCD_WriteText("l", 1, 1, 0);
                Driver_LCD_WriteUInt(ConvertedData.Distance.left, 1, 8);
                Driver_LCD_WriteText("r", 1, 2, 0);
                Driver_LCD_WriteUInt(ConvertedData.Distance.right, 2, 8);
                Driver_LCD_WriteUInt(ADC12Data.ADCBuffer[3], 7, 0);
                DriveStatus.refreshCount = 0;
            } else {
                DriveStatus.refreshCount += 1;
            }
            DriveStatus.requested = 0;
	    }
	    if (BUTTONCOM.active == 1) {
	        if (BUTTONCOM.button == 0) {
	            DriveStatus.start = 1;
	        } else if (BUTTONCOM.button == 1){
	            DriveStatus.start = 0;
	        }
	        BUTTONCOM.active = 0;
	    }
	}
}
