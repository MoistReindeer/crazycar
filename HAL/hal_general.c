#include "HAL/hal_general.h"
#include "HAL/hal_pmm.h"
#include "HAL/hal_wdt.h"
#include "HAL/hal_gpio.h"
#include <msp430.h>

void HAL_Init(void) {
    HAL_Wdt_Init();
    HAL_PMM_Init();
    HAL_GPIO_Init();
}
