#include <msp430.h>
#include <DL/driver_general.h>
#include <DL/driver_aktorik.h>
#include <DL/driver_lcd.h>

void Driver_Init(void) {
    Driver_SteeringInit();
    //Driver_ESCInit();
    Driver_LCD_Init();
}
