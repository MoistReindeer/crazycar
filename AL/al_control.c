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

short steeringValue = 0;
short curveDelay = 0;

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
    Driver_SetThrottle(60);
}

void AL_Control_Steer() {
    parameters.e = ConvertedData.Distance.right - ConvertedData.Distance.left; // +400 Aligns the car with the right wall
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
}

void AL_Fetch_Direction() {
    short diff = ConvertedData.Distance.right - ConvertedData.Distance.left;
    short sum = ConvertedData.Distance.right + ConvertedData.Distance.left;

    switch (DriveStatus.Steer.curr) {
        case FORWARD:
            if (diff < (-DEAD_ZONE - 450))
                DriveStatus.Steer.curr = RIGHT;
            else if (diff > (DEAD_ZONE + 450))
                DriveStatus.Steer.curr = LEFT;
            else
                steeringValue = parameters.y >> 1;
            break;
        case LEFT:
            /*if (parameters.y <= 50) { // Basic Correction, keine Kurve
                steeringValue = parameters.y;
            }*/
            if (sum <= ConvertedData.Distance.front)
                DriveStatus.Steer.curr = FORWARD;
            else
                steeringValue = 100;
            break;
        case RIGHT:
            if (sum <= ConvertedData.Distance.front)
                DriveStatus.Steer.curr = FORWARD;
            else
                steeringValue = -100;
            break;
        case CORRECTION:
            steeringValue = parameters.y;
    }
    if (ConvertedData.Distance.front <= 250) {
        Driver_SetThrottle(0);
    }
    Driver_SetSteering(steeringValue);
    Driver_LCD_WriteUInt(DriveStatus.Steer.curr, 3, 0);
    Driver_LCD_WriteUInt(diff, 5, 0);
    Driver_LCD_WriteUInt(parameters.y, 6, 0);
    return;
}
