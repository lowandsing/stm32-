    #ifndef USART_H
    #define USART_H

    #include "stm32f1xx_hal.h"  //  正确包含HAL库核心头文件，它包含了UART_HandleTypeDef的定义
    #include "stm32f1xx_hal_uart.h"
    #include "stm32f1xx_hal_exti.h"
    extern UART_HandleTypeDef g_usart_handle;
    extern uint8_t g_rx_buffer[1];//缓存
    extern uint8_t g_uart_1_rx_flag;//串口接收到数据标志
    void uart_Init(uint32_t bound);

    #endif // USART_H
