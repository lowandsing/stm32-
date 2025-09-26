//
// Created by lg on 25-9-13.
//
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_tim.h"
#ifndef ATIM_H
#define ATIM_H

void atim_timx_comp_pwm_init(uint16_t arr, uint16_t psc);
#endif //ATIM_H
