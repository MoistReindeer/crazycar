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

void Driver_SetThrottle(int throttle) { /* Beschleunigen/Bremsen pro step 25µs*/
    if (throttle >= 100) {
        TA1CCR1 = 10000;
    } else if (throttle <= -100) {
        TA1CCR1 = 2500;
    } else if (throttle > 0) {
        TA1CCR1 = 7500 + 25*throttle;
    } else if (throttle < 0) {
        TA1CCR1 = 5000 - 25*throttle;
    } else if (throttle == 0) {
        TA1CCR1 = 6250;
    }
}

void Driver_ESCInit(void) {
    createPulses(MAX_RPW,131);
    createPulses(MIN_RPW,128);

    createPulses(MIN_FPW,128);
    createPulses(MAX_FPW,128);

    createPulses(MAX_BREAK, 30);
}

void createPulses(int pwm, int pulseDuration)
{
    speed_controller_impuls = 0;
    TA1CCR1 = pwm;
    while(speed_controller_impuls <= pulseDuration)
    {

    }
}

# pragma vector = TIMER1_A1_VECTOR; // pending , when TA1R counts to
__interrupt void TimerA (void)
{
    speed_controller_impuls ++;
    TA1IV = 0x00;
}
