#include <msp430.h>
#include <HAL/hal_gpio.h>
#include <DL/driver_lcd.h>
#include <AL/al_conv.h>
#include <AL/al_control.h>
#include <DL/driver_aktorik.h>
#include <HAL/hal_adc12.h>

#define DEAD_ZONE 475

extern ConversionData ConvertedData;
extern ADC12Com ADC12Data;
DriveStatus_t DriveStatus;
PIParams_t parameters;

short steeringValue = 0;
short curveDelay = 0;

void AL_Param_Init() {
    parameters.Steer.kp = 0.25;
    parameters.Steer.ki = 0.03;
    parameters.Steer.kd = 0.09;
    parameters.Steer.esum = 0;
    parameters.Steer.ta = 0.1;
    parameters.Steer.satLow = -60;
    parameters.Steer.satHigh = 60;
}

void AL_Control_Drive() {
    Driver_SetThrottle(60);
}

void AL_Average_Sensors() {
    unsigned int lbuff = 0, rbuff = 0, fbuff = 0, i;
    for (i = 7; i == 0; i--) {
        //while (ADC12Data.Status.B.ADCrdy != 1);
        lbuff += ADC12Data.SensorLeft;
        rbuff += ADC12Data.SensorRight;
        fbuff += ADC12Data.SensorFront;
    }

    ConvertedData.Distance.left = ADC12Data.SensorLeft; // >> 3;
    ConvertedData.Distance.right = ADC12Data.SensorRight; // >> 3;
    ConvertedData.Distance.front = ADC12Data.SensorFront; // >> 3;
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
    AL_Average_Sensors();
    AL_Control_Steer();
    short diff = ConvertedData.Distance.right - ConvertedData.Distance.left;
    short sum = ConvertedData.Distance.right + ConvertedData.Distance.left;

    switch (DriveStatus.Steer.curr) {
        case FORWARD:
            if (diff < -DEAD_ZONE) {
                DriveStatus.Steer.curr = LEFT;
                if (DriveStatus.start == 0)
                    Driver_SetThrottle(0);
                else {
                    Driver_SetThrottle(36);
                }
            } else if (diff > DEAD_ZONE) {
                DriveStatus.Steer.curr = RIGHT;
                if (DriveStatus.start == 0)
                    Driver_SetThrottle(0);
                else {
                    Driver_SetThrottle(36);
                }
            } else {
                steeringValue = parameters.Steer.y;
                if (DriveStatus.start == 0)
                    Driver_SetThrottle(0);
                else
                    Driver_SetThrottle(53);
            }
            break;
        case LEFT:
            if (sum <= ConvertedData.Distance.front && diff > -DEAD_ZONE) {
                DriveStatus.Steer.count += 1;
                DriveStatus.Steer.curr = FORWARD;
            } else
                steeringValue = -100;
            break;
        case RIGHT:
            if (DriveStatus.Steer.count == 2 && ConvertedData.Distance.front >= 1000 && ConvertedData.Distance.front <= 1200) {
                DriveStatus.Steer.curr = FORWARD;
                DriveStatus.Steer.circle = 1;
            } else if (sum <= ConvertedData.Distance.front && diff < DEAD_ZONE) {
                DriveStatus.Steer.count += 1;
                DriveStatus.Steer.curr = FORWARD;
            } else
                steeringValue = 100;
            break;
        case CORRECTION:
            steeringValue = parameters.Steer.y >> 1;
    }
    Driver_SetSteering(steeringValue);
    if (DriveStatus.refreshCount >= 120) {
        Driver_LCD_WriteText("circle", 7, 5, 0);
        Driver_LCD_WriteUInt(DriveStatus.Steer.circle, 5, 49);
        Driver_LCD_WriteText("cnt", 3, 6, 0);
        Driver_LCD_WriteUInt(DriveStatus.Steer.count, 6, 49);
        Driver_LCD_WriteText("ar", 2, 3, 0);
        Driver_LCD_WriteUInt(DriveStatus.arear, 3, 49);
        Driver_LCD_WriteText("al", 2, 4, 0);
        Driver_LCD_WriteUInt(DriveStatus.areal, 4, 49);
        /*Driver_LCD_WriteText("stat", 4, 3, 0);
        Driver_LCD_WriteUInt(DriveStatus.Drive.curr, 3, 49);*/
    }
    if (DriveStatus.Steer.count >= 8)
        DriveStatus.Steer.count = 0;
    return;
}
