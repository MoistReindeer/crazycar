#ifndef _AL_CONTROL_H_
#define _AL_CONTROL_H_

enum States{FORWARD, LEFT, RIGHT, START, STOP};

typedef struct {
    struct {
        unsigned short curr; // Last set steering direction
        unsigned short set; // To be set steering direction
    } Steer;
    struct {
        short curr;
        short set;
    } Drive;
} DriveStatus_t;

typedef struct {
    float kp, ki, kd, esum, ta;
    short e, e_old, y, satLow, satHigh;
} PIParams_t;

void AL_Param_Init();
void AL_Fetch_Direction();

#endif /* _AL_CONTROL_H_ */
