#include "HAL/hal_wdt.h"
#include <msp430.h>

void HAL_Wdt_Init(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
}
