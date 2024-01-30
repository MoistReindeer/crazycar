#include <msp430.h>
#include <HAL/hal_gpio.h>
#include <DL/driver_lcd.h>
#include <AL/al_conv.h>
#include <AL/al_control.h>
#include <DL/driver_aktorik.h>
#include <HAL/hal_adc12.h>

#define DEAD_ZONE 425
#define MINSPD 33
#define MAXSPD 54
#define LOCK_MAX 45

extern ConversionData ConvertedData;
extern ADC12Com ADC12Data;
DriveStatus_t DriveStatus;
PIParams_t parameters;

unsigned short lockCnt = 0;
short steeringValue = 0;
short curveDelay = 0;

void AL_Param_Init() {
    parameters.Steer.kp = 0.09;
    parameters.Steer.ki = 0;
    parameters.Steer.kd = 0.05;
    parameters.Steer.esum = 0;
    parameters.Steer.ta = 0.1;
    parameters.Steer.satLow = -60;
    parameters.Steer.satHigh = 60;

    parameters.Drive.kp = 0.05;
    parameters.Drive.ki = 0.04;
    parameters.Drive.kd = 0.01;
    parameters.Drive.esum = 0;
    parameters.Drive.ta = 0.1;
    parameters.Drive.satLow = -100;
    parameters.Drive.satHigh = 60;

    // Set default parameters
    DriveStatus.Steer.curr = FORWARD;
    DriveStatus.refreshCount = 0;
    DriveStatus.start = 0;
    DriveStatus.Speed.minSpd = MINSPD;
    DriveStatus.Speed.maxSpd = 56;
    DriveStatus.Steer.count = 0;
    DriveStatus.Count.l = 0;
    DriveStatus.Count.r = 0;
}

void AL_Control_Drive() {
    short speed = 0;
    switch (DriveStatus.Steer.curr) {
        case FORWARD:
            speed = front_speed_tbl[ConvertedData.Distance.front >> 3];
            break;
        case LEFT:
            speed = front_speed_tbl[((ConvertedData.Distance.left + ConvertedData.Distance.front) >> 1) >> 3];
            break;
        case RIGHT:
            speed = front_speed_tbl[((ConvertedData.Distance.right + ConvertedData.Distance.front) >> 1) >> 3];
            break;
    }

    if (speed > DriveStatus.Speed.maxSpd)
        speed = DriveStatus.Speed.maxSpd;
    else if (speed < DriveStatus.Speed.minSpd)
        speed = DriveStatus.Speed.minSpd;
    if (ConvertedData.Distance.front <= 90)
        speed = 0;
    Driver_LCD_WriteText("spd", 3, 3, 0);
    Driver_LCD_WriteUInt(speed, 3, 49);
    Driver_SetThrottle(speed);
    DriveStatus.Speed.value = speed;
}

void AL_Average_Sensors() {
    unsigned int lbuff = 0, rbuff = 0, fbuff = 0, i;
    for (i = 7; i == 0; i--) {
        while (ADC12Data.Status.B.ADCrdy != 1);
        lbuff += ADC12Data.SensorLeft;
        rbuff += ADC12Data.SensorRight;
        fbuff += ADC12Data.SensorFront;
    }

    ConvertedData.Distance.left = ADC12Data.SensorLeft; // >> 3;
    ConvertedData.Distance.right = ADC12Data.SensorRight; // >> 3;
    ConvertedData.Distance.front = ADC12Data.SensorFront; // >> 3;

}

void AL_Control_Steer() {
    parameters.Steer.e = (ConvertedData.Distance.right - ConvertedData.Distance.left) >> 4; // +400 Aligns the car with the right wall; division by 17 (16) for 0-100% scaling
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
            if (diff < -DEAD_ZONE && ConvertedData.Distance.left > 800) {
                if (lockCnt < LOCK_MAX && DriveStatus.Count.l > 0)
                    DriveStatus.Count.l -= 1;
                DriveStatus.Steer.curr = LEFT;
                DriveStatus.Count.l += 1;
                lockCnt = 0;
            } else if (diff > DEAD_ZONE && ConvertedData.Distance.right > 800) {
                if (lockCnt < LOCK_MAX && DriveStatus.Count.r > 0)
                    DriveStatus.Count.r -= 1;
                DriveStatus.Steer.curr = RIGHT;
                DriveStatus.Count.r += 1;
                lockCnt = 0;
            } else {
                steeringValue = parameters.Steer.y;
            }
            break;
        case LEFT:
            if (sum <= ConvertedData.Distance.front + 250 && diff > -DEAD_ZONE) {
                DriveStatus.Steer.curr = FORWARD;
            } else {
                lockCnt += 1;
                steeringValue = -100;
            }
            break;
        case RIGHT:
            if (sum <= ConvertedData.Distance.front + 250 && diff < DEAD_ZONE) {
                DriveStatus.Steer.curr = FORWARD;
            } else {
                lockCnt += 1;
                steeringValue = 100;
            }
            break;
    }

    // Set Steering
    Driver_SetSteering(steeringValue);

    // Display Variables
    if (DriveStatus.refreshCount >= 120) {
        //Driver_LCD_WriteText("circle", 7, 5, 0);
        //Driver_LCD_WriteUInt(DriveStatus.Steer.circle, 5, 49);
        Driver_LCD_WriteText("cnt", 3, 6, 0);
        Driver_LCD_WriteUInt(lockCnt, 6, 49);
        Driver_LCD_WriteText("rc", 2, 4, 0);
        Driver_LCD_WriteUInt(DriveStatus.Count.r, 4, 49);
        Driver_LCD_WriteText("lc", 2, 5, 0);
        Driver_LCD_WriteUInt(DriveStatus.Count.l, 5, 49);
        /*Driver_LCD_WriteText("stat", 4, 3, 0);
        Driver_LCD_WriteUInt(DriveStatus.Drive.curr, 3, 49);*/
    }
    return;
}
