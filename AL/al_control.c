#include <msp430.h>
#include <HAL/hal_gpio.h>
#include <DL/driver_lcd.h>
#include <AL/al_conv.h>

extern ConversionData ConvertedData;

void AL_Fetch_Direction() {
    short direction = ConvertedData.Distance.right - ConvertedData.Distance.left;
    if (direction < -75) {
        Driver_LCD_WriteText("LEFT", 4, 3, 0);
    } else if (direction > 75) {
        Driver_LCD_WriteText("RIGHT", 5, 3, 0);
    } else {
        Driver_LCD_WriteText("STRAIGHT", 8, 3, 0);
    }
    return;
}
