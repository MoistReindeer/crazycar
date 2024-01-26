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
    Driver_SetThrottle(60);
}

void AL_Control_Steer() {
    if (ConvertedData.Distance.right > ConvertedData.Distance.left) {
        parameters.e = ConvertedData.Distance.right - ConvertedData.Distance.left;
    } else {
        parameters.e = ConvertedData.Distance.left - ConvertedData.Distance.right;
    }
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
    short steeringValue = 0;
    if ((ConvertedData.Distance.right >= ConvertedData.Distance.front) || (ConvertedData.Distance.left >= ConvertedData.Distance.front)) {
        DriveStatus.Steer.uTurn = 1;
    } else if (DriveStatus.Steer.set == FORWARD && (ConvertedData.velocity_dd > 200)) {
        DriveStatus.Steer.uTurn = 0;
    }
    Driver_LCD_WriteUInt(DriveStatus.Steer.uTurn, 5, 0);

    if ((direction < -DEAD_ZONE) || ((DriveStatus.Steer.uTurn == 1)) && (DriveStatus.Steer.curr == LEFT)) {
        DriveStatus.Steer.set = LEFT;
        steeringValue = parameters.y*-1;
    } else if ((direction > DEAD_ZONE) || ((DriveStatus.Steer.uTurn == 1)) && (DriveStatus.Steer.curr == RIGHT)) {
        DriveStatus.Steer.set = RIGHT;
        steeringValue = parameters.y;
    } else {
        DriveStatus.Steer.set = FORWARD;
        steeringValue = 0;
    }

    Driver_SetSteering(steeringValue);
    Driver_LCD_WriteUInt(DriveStatus.Steer.set, 3, 0);
    return;
}
