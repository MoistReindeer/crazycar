#include <msp430.h>
#include <HAL/hal_gpio.h>
#include <DL/driver_lcd.h>
#include <AL/al_conv.h>
#include <AL/al_control.h>
#include <DL/driver_aktorik.h>

#define DEAD_ZONE 75

extern ConversionData ConvertedData;
DriveStatus_t DriveStatus;
PIParams_t parameters;

void AL_Param_Init() {
    parameters.kp = 0.25;
    parameters.ki = 0.01;
    parameters.kd = 0.02;
    parameters.esum = 0;
    parameters.ta = 0.1;
    parameters.satLow = -100;
    parameters.satHigh = 100;
}

void AL_Control_Drive() {

}

void AL_Control_Steer() {
    parameters.e = ConvertedData.Distance.right - ConvertedData.Distance.left;
    if ((parameters.y > parameters.satLow) && (parameters.y < parameters.satHigh)) {
        parameters.esum += parameters.e;
    }
    parameters.y = parameters.kp * parameters.e;
    parameters.y += parameters.ki * parameters.ta * parameters.esum;
    parameters.y += parameters.kd * (parameters.e - parameters.e_old) / parameters.ta;
    parameters.e_old = parameters.e;

    if (parameters.y < parameters.satLow) {
        parameters.y = parameters.satLow;
    } else if (parameters.y > parameters.satHigh) {
        parameters.y = parameters.satHigh;
    }
    Driver_SetSteering(parameters.y);
}

void AL_Fetch_Direction() {
    short direction = ConvertedData.Distance.right - ConvertedData.Distance.left;
    if (direction < -DEAD_ZONE) {
        DriveStatus.Steer.set = LEFT;
    } else if (direction > DEAD_ZONE) {
        DriveStatus.Steer.set = RIGHT;
    } else {
        DriveStatus.Steer.set = FORWARD;
    }
    Driver_LCD_WriteUInt(DriveStatus.Steer.set, 3, 0);
    return;
}
