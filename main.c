#include <msp430.h> 
#include <HAL/hal_general.h>
#include <HAL/hal_gpio.h>
#include <HAL/hal_pmm.h>

extern ButtonCom BUTTONCOM;

/**
 * main.c
 */
void main(void)
{
    HAL_Init();
	
	while (1)
	{
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
