#include <msp430.h>
#include <DL/driver_aktorik.h>
#include <DL/driver_general.h>

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

void Driver_ESCInit(void) {
    Driver_SetThrottle(50);
    __delay_cycles(47648000); //140 Impulse * 16,66 / MCLK 1/20MHz
    Driver_SetThrottle(-100);
    __delay_cycles(47648000);
    Driver_SetThrottle(-1);
    __delay_cycles(47648000);
    Driver_SetThrottle(1);
    __delay_cycles(47648000);
    Driver_SetThrottle(100);
    __delay_cycles(47648000);
    Driver_SetThrottle(0);
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
