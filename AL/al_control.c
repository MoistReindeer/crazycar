#include <msp430.h>
#include <HAL/hal_gpio.h>
#include <DL/driver_lcd.h>
#include <AL/al_conv.h>
#include <AL/al_control.h>
#include <DL/driver_aktorik.h>

#define DEAD_ZONE 475

extern ConversionData ConvertedData;
DriveStatus_t DriveStatus;
PIParams_t parameters;

short steeringValue = 0;
short curveDelay = 0;

void AL_Param_Init() {
    parameters.kp = 0.25;
    parameters.ki = 0.04;
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
    AL_Control_Steer();
    short diff = ConvertedData.Distance.right - ConvertedData.Distance.left;
    short sum = ConvertedData.Distance.right + ConvertedData.Distance.left;
    short circ = (ConvertedData.Distance.right + ConvertedData.Distance.front) >> 1;

    if (ConvertedData.Distance.right >= 1500 && ConvertedData.Distance.front <= 1200 && ConvertedData.Distance.front >= 1100 && ConvertedData.Distance.left <= 800 && ConvertedData.Distance.right >= 600)
        DriveStatus.Steer.circle = 1;

    switch (DriveStatus.Steer.curr) {
        case FORWARD:
            if (diff < -DEAD_ZONE) {
                DriveStatus.Steer.curr = RIGHT;
                if (DriveStatus.start == 0)
                    Driver_SetThrottle(0);
                else {
                    DriveStatus.Steer.count += 1;
                    Driver_SetThrottle(45);
                }
            } else if (diff > DEAD_ZONE) {
                DriveStatus.Steer.curr = LEFT;
                if (DriveStatus.start == 0)
                    Driver_SetThrottle(0);
                else {
                    DriveStatus.Steer.count += 1;
                    Driver_SetThrottle(45);
                }
            } else {
                steeringValue = parameters.y >> 1;
                if (DriveStatus.start == 0)
                    Driver_SetThrottle(0);
                else
                    Driver_SetThrottle(55);
            }
            break;
        case LEFT:
            if (sum <= ConvertedData.Distance.front)
                DriveStatus.Steer.curr = FORWARD;
            else
                steeringValue = 100;
            break;
        case RIGHT:
            if (sum <= ConvertedData.Distance.front) {
                DriveStatus.Steer.curr = FORWARD;
                steeringValue = 0;
            } else
                steeringValue = -100;
            break;
        case CORRECTION:
            steeringValue = parameters.y;
    }
    /*if (ConvertedData.Distance.front <= 175) {
        Driver_SetThrottle(0);
    }*/
    //AL_Control_Drive();
    /*if (DriveStatus.Steer.circle != 1) {
        DriveStatus.Steer.curr = FORWARD;
        steeringValue = 0;
    }*/
    Driver_SetSteering(steeringValue);
    Driver_LCD_WriteUInt(DriveStatus.Steer.curr, 3, 0);
    Driver_LCD_WriteUInt(DriveStatus.Steer.circle, 5, 0);
    Driver_LCD_WriteUInt(DriveStatus.Steer.count, 6, 0);
    return;
}
