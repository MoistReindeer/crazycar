#include <msp430.h> 
#include <HAL/hal_general.h>
#include <HAL/hal_gpio.h>

/**
 * main.c
 */
void main(void)
{
    float f_erg;
    int i_erg;

    HAL_Init();
	
	while (1)
	{
	    // Floating-point Test - 10.9µs
	    LCD_BACKLIGHT_ON;
	    f_erg = 4.978f*2.673489f;

	    // Integer Test - 8.2µs
        LCD_BACKLIGHT_OFF;
        i_erg = 4*2;
	}
}
