#ifndef _AL_CONTROL_H_
#define _AL_CONTROL_H_

enum States{FORWARD, LEFT, RIGHT, CORRECTION, START, STOP};
enum Curves{CV1, CV2, CV3, CV4, CV5, CV6, CV7, CV8, CV9};

typedef struct {
    struct {
        unsigned short curr; // Last set steering direction
        unsigned short set; // To be set steering direction
        unsigned char uturn; // 0 if not, 1 if U-Turn
        unsigned char circle; // Used for kreisel, 1 if detected, 0 if not
        unsigned int count;
        short curve; // Currend driven curve
        short align; // Value to align the car in the straight
    } Steer;
    struct {
        short curr;
        short set;
        unsigned short front_old;
        unsigned short left_old;
        unsigned short right_old;
    } Drive;
    struct {
        unsigned short value;
        unsigned short maxSpd;
        unsigned short minSpd;
    } Speed;
    struct {
        unsigned short r;
        unsigned short l;
        unsigned short round;
    } Count;
    char start;
    char requested;
    unsigned short refreshCount;
    unsigned int areal, arear;
} DriveStatus_t;

typedef struct {
    struct {
        float kp, ki, kd, esum, ta;
        short e, e_old, y, satLow, satHigh;
    } Steer;
    struct {
        float kp, ki, kd, esum, ta;
        short e, e_old, y, satLow, satHigh;
    } Drive;
} PIParams_t;

void AL_Param_Init();
void AL_Fetch_Direction();

#endif /* _AL_CONTROL_H_ */
