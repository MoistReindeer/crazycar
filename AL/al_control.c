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
    parameters.Steer.kp = 0.25;
    parameters.Steer.ki = 0.04;
    parameters.Steer.kd = 0.02;
    parameters.Steer.esum = 0;
    parameters.Steer.ta = 0.1;
    parameters.Steer.satLow = -100;
    parameters.Steer.satHigh = 100;
}

void AL_Control_Drive() {
    Driver_SetThrottle(60);
}

void AL_Control_Steer() {
    parameters.Steer.e = ConvertedData.Distance.right - ConvertedData.Distance.left; // +400 Aligns the car with the right wall
    if ((parameters.Steer.y > parameters.Steer.satLow) && (parameters.Steer.y < parameters.Steer.satHigh)) {
        parameters.Steer.esum += parameters.Steer.e;
    }
    parameters.Steer.y = parameters.Steer.kp * parameters.Steer.e;
    parameters.Steer.y += parameters.Steer.ki * parameters.Steer.ta * parameters.Steer.esum;
    parameters.Steer.y += parameters.Steer.kd * (parameters.Steer.e - parameters.Steer.e_old) / parameters.Steer.ta;
    parameters.Steer.e_old = parameters.Steer.e;

    if (parameters.Steer.y < parameters.Steer.satLow) {
        parameters.Steer.y = parameters.Steer.satLow;
    } else if (parameters.Steer.y > parameters.Steer.satHigh) {
        parameters.Steer.y = parameters.Steer.satHigh;
    }
}

void AL_Fetch_Direction() {
    AL_Control_Steer();
    short diff = ConvertedData.Distance.right - ConvertedData.Distance.left;
    short sum = ConvertedData.Distance.right + ConvertedData.Distance.left;
    short area = (ConvertedData.Distance.right * ConvertedData.Distance.front * 0.85) >> 1 + (ConvertedData.Distance.left * ConvertedData.Distance.front * 0.85) >> 1; // 0.85 = sin(~45°)
    short circ = (ConvertedData.Distance.right + ConvertedData.Distance.front) >> 1;

    if (DriveStatus.Steer.count >= 0 && DriveStatus.Steer.count < 4){
        if (ConvertedData.Distance.front >= 1000 && ConvertedData.Distance.front <= 1500 && ConvertedData.Distance.left >= 200 && ConvertedData.Distance.left <= 800 && ConvertedData.Distance.right >= 1100) {
            DriveStatus.Steer.circle = 1;
        }
//.right >= 1500 && ConvertedData.Distance.front <= 1200 && ConvertedData.Distance.front >= 1100 && ConvertedData.Distance.left <= 800 && ConvertedData.Distance.right >= 600)
    }

    switch (DriveStatus.Steer.curr) {
        case FORWARD:
            if (diff < -DEAD_ZONE) {
                DriveStatus.Steer.curr = RIGHT;
                if (DriveStatus.start == 0)
                    Driver_SetThrottle(0);
                else {
                    DriveStatus.Steer.count += 1;
                    Driver_SetThrottle(40);
                }
            } else if (diff > DEAD_ZONE) {
                DriveStatus.Steer.curr = LEFT;
                if (DriveStatus.start == 0)
                    Driver_SetThrottle(0);
                else {
                    DriveStatus.Steer.count += 1;
                    Driver_SetThrottle(40);
                }
            } else {
                steeringValue = parameters.Steer.y >> 1;
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
            DriveStatus.Steer.circle = 0;
            break;
        case RIGHT:
            if (sum <= ConvertedData.Distance.front) {
                DriveStatus.Steer.curr = FORWARD;
            } else
                steeringValue = -100;
            break;
        case CORRECTION:
            steeringValue = parameters.Steer.y;
    }
    /*if (ConvertedData.Distance.front <= 175) {
        Driver_SetThrottle(0);
    }*/
    //AL_Control_Drive();
    if (DriveStatus.Steer.circle == 1) {
        DriveStatus.Steer.curr = FORWARD;
        steeringValue = 0;
    }
    Driver_SetSteering(steeringValue);
    Driver_LCD_WriteUInt(area, 3, 0);
    Driver_LCD_WriteUInt(DriveStatus.Steer.circle, 5, 0);
    Driver_LCD_WriteUInt(DriveStatus.Steer.count, 6, 0);
    return;
}
