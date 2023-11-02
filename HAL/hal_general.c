#include "HAL/hal_general.h"
#include "HAL/hal_pmm.h"
#include "HAL/hal_wdt.h"
#include "HAL/hal_gpio.h"
#include "HAL/hal_ucs.h"
#include "HAL/hal_timerA1.h"
#include "HAL/hal_timerB0.h"
#include <msp430.h>

ButtonCom BUTTONCOM;

void HAL_Init(void) {
    HAL_Wdt_Init();
    HAL_PMM_Init();
    HAL_GPIO_Init();
    HAL_UCS_Init();
    HAL_TimerA1_Init();
    HAL_TimerB0_Init();
    Driver_SteeringInit();
}
