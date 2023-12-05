#include <msp430.h>
#include <DL/driver_aktorik.h>
#include <DL/driver_general.h>

int speed_controller_impuls = 0;

void Driver_SteeringInit(void) {
    Driver_SetSteering(0);
}

void Driver_SetSteering(int steer) { // Rechts-Links lenken
    if (steer == 0){
        TA1CCR2 = STEER_MIDDLE;
    } else if (steer >= 100) {
        TA1CCR2 = STEER_RIGHT;
    } else if (steer <=-100) {
        TA1CCR2 = STEER_LEFT;
    } else {
        TA1CCR2 = STEER_MIDDLE + 6*steer;
    }
}

void Driver_SetThrottle(int throttle) { /* Beschleunigen/Bremsen pro step 25�s*/
    if (throttle >= 100) {
        TA1CCR1 = MAX_FPW;
    } else if (throttle <= -100) {
        TA1CCR1 = MAX_RPW;
    } else if (throttle > 0) {
        TA1CCR1 = MIN_FPW + 25*throttle;
    } else if (throttle < 0) {
        TA1CCR1 = MIN_RPW - 25*throttle;
    } else {
        TA1CCR1 = MAX_BREAK;
    }
}

void Driver_ESCInit(void) {
    createPulses(MAX_RPW,140);
    createPulses(MIN_RPW,140);

    createPulses(MIN_FPW,140);
    createPulses(MAX_FPW,140);

    createPulses(MAX_BREAK, 60);
}

void createPulses(int pwm, int pulseDuration)
{
    speed_controller_impuls = 0;
    TA1CCR1 = pwm;
    while(speed_controller_impuls <= pulseDuration)
    {

    }
}

# pragma vector = TIMER1_A0_VECTOR; // pending , when TA1R counts to
__interrupt void TimerA (void)
{
    speed_controller_impuls ++;
}
