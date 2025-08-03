//
// Created by lg on 25-7-20.
//

#ifndef SERIAL_H
#define SERIAL_H
#include "stdio.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_uart.h"
void Serial_Init(void);
void Serial_send_byte(uint8_t byte);
void test_serial_send_string(void);
#endif //SERIAL_H
