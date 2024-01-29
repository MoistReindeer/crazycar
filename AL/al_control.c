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
    parameters.Steer.ki = 0.04;
    parameters.Steer.kd = 0.02;
    parameters.Steer.esum = 0;
    parameters.Steer.ta = 0.1;
    parameters.Steer.satLow = -60;
    parameters.Steer.satHigh = 60;

    parameters.Drive.kp = 0.05;
    parameters.Drive.ki = 0.04;
    parameters.Drive.kd = 0.30;
    parameters.Drive.esum = 0;
    parameters.Drive.ta = 0.1;
    parameters.Drive.satLow = -100;
    parameters.Drive.satHigh = 60;
}

void AL_Control_Drive() {
    /*
    parameters.Drive.e = (ConvertedData.Distance.front - DriveStatus.Drive.front_old + ConvertedData.Distance.left - DriveStatus.Drive.left_old + ConvertedData.Distance.right - DriveStatus.Drive.right_old)/3;
    if ((parameters.Drive.y > parameters.Drive.satLow) && (parameters.Drive.y < parameters.Drive.satHigh)) {
        parameters.Drive.esum += parameters.Drive.e;
    }
    parameters.Drive.y = parameters.Drive.kp * parameters.Drive.e;
    parameters.Drive.y += parameters.Drive.ki * parameters.Drive.ta * parameters.Drive.esum;
    parameters.Drive.y += parameters.Drive.kd * (parameters.Drive.e - parameters.Drive.e_old) / parameters.Drive.ta;
    parameters.Drive.e_old = parameters.Drive.e;

    if (parameters.Drive.y < parameters.Drive.satLow) {
        parameters.Drive.y = parameters.Drive.satLow;
    } else if (parameters.Drive.y > parameters.Drive.satHigh) {
        parameters.Drive.y = parameters.Drive.satHigh;
    }

    Driver_SetThrottle(parameters.Drive.y);
    DriveStatus.Drive.front_old = ConvertedData.Distance.front;
    DriveStatus.Drive.left_old = ConvertedData.Distance.left;
    DriveStatus.Drive.right_old = ConvertedData.Distance.right;*/
    unsigned short speed = 0;
    switch (DriveStatus.Steer.curr) {
            case FORWARD:
                speed = front_speed_tbl[ConvertedData.Distance.front >> 3];
                break;
            case LEFT:
                speed = front_speed_tbl[((ConvertedData.Distance.left + ConvertedData.Distance.front) >> 2) >> 3];
                break;
            case RIGHT:
                speed = front_speed_tbl[((ConvertedData.Distance.right + ConvertedData.Distance.front) >> 2) >> 3];
                break;
    }
    if (speed > 65)
        speed = 65;
    else if (speed < 30)
        speed = 40;
    if (ConvertedData.Distance.front <= 100)
        speed = 0;
    Driver_SetThrottle(speed);
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
    if (DriveStatus.start != 0)
        AL_Control_Drive();
    short diff = ConvertedData.Distance.right - ConvertedData.Distance.left;
    short sum = ConvertedData.Distance.right + ConvertedData.Distance.left;

    switch (DriveStatus.Steer.curr) {
        case FORWARD:
            if (diff < -DEAD_ZONE) {
                DriveStatus.Steer.curr = LEFT;
                /*if (DriveStatus.start == 0)
                    Driver_SetThrottle(0);
                else {
                    Driver_SetThrottle(36);
                }*/
            } else if (diff > DEAD_ZONE) {
                DriveStatus.Steer.curr = RIGHT;
                /*if (DriveStatus.start == 0)
                    Driver_SetThrottle(0);
                else {
                    Driver_SetThrottle(36);
                }*/
            } else {
                steeringValue = parameters.Steer.y;
                /*if (DriveStatus.start == 0)
                    Driver_SetThrottle(0);
                else
                    Driver_SetThrottle(53);*/
            }
            break;
        case LEFT:
            if (sum <= ConvertedData.Distance.front || diff > -DEAD_ZONE) {
                DriveStatus.Steer.count += 1;
                DriveStatus.Steer.curr = FORWARD;
            } else
                steeringValue = -100;
            break;
        case RIGHT:
            if (DriveStatus.Steer.count == 2 || ConvertedData.Distance.front >= 1000 && ConvertedData.Distance.front <= 1200) {
                DriveStatus.Steer.curr = FORWARD;
                DriveStatus.Steer.circle = 1;
            } else if (sum <= ConvertedData.Distance.front && diff < DEAD_ZONE) {
                DriveStatus.Steer.count += 1;
                DriveStatus.Steer.curr = FORWARD;
            } else
                steeringValue = 100;
            break;
        case CORRECTION:
            steeringValue = parameters.Steer.y;
    }
    Driver_SetSteering(steeringValue);
    if (DriveStatus.refreshCount >= 120) {
        Driver_LCD_WriteText("circle", 7, 5, 0);
        Driver_LCD_WriteUInt(DriveStatus.Steer.circle, 5, 49);
        Driver_LCD_WriteText("cnt", 3, 6, 0);
        Driver_LCD_WriteUInt(DriveStatus.Steer.count, 6, 49);
        /*Driver_LCD_WriteText("stat", 4, 3, 0);
        Driver_LCD_WriteUInt(DriveStatus.Drive.curr, 3, 49);*/
    }
    if (DriveStatus.Steer.count >= 8)
        DriveStatus.Steer.count = 0;
    return;
}
