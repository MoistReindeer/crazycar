#ifndef _AL_CONTROL_H_
#define _AL_CONTROL_H_

enum States{FORWARD, LEFT, RIGHT, CORRECTION, START, STOP};

typedef struct {
    struct {
        unsigned short curr; // Last set steering direction
        unsigned short set; // To be set steering direction
        unsigned char uturn; // 0 if not, 1 if U-Turn
        unsigned char circle; // Used for kreisel, 1 if detected, 0 if not
        unsigned short count;
    } Steer;
    struct {
        short curr;
        short set;
    } Drive;
    char start;
    char requested;
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
