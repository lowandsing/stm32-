#include <string.h>

#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_uart.h"  // 改成UART头文件

UART_HandleTypeDef huart1;

void Serial_Init(void)
{
 __HAL_RCC_GPIOA_CLK_ENABLE();
 __HAL_RCC_USART1_CLK_ENABLE();

 GPIO_InitTypeDef GPIO_InitStruct = {0};

 GPIO_InitStruct.Pin = GPIO_PIN_9;
 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 GPIO_InitStruct.Pin = GPIO_PIN_10;
 GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 huart1.Instance = USART1;
 huart1.Init.BaudRate = 9600;
 huart1.Init.WordLength = UART_WORDLENGTH_8B;
 huart1.Init.StopBits = UART_STOPBITS_1;
 huart1.Init.Parity = UART_PARITY_NONE;
 huart1.Init.Mode = UART_MODE_TX_RX;
 huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
 huart1.Init.OverSampling = UART_OVERSAMPLING_16;
 HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
 if (HAL_UART_Init(&huart1) != HAL_OK)
 {
  Error_Handler();
 }
}

void Serial_send_byte(uint8_t byte)
{
 HAL_UART_Transmit(&huart1, &byte, 1, HAL_MAX_DELAY);
 HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
 while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) == RESET);
}
// void test_serial_send_string(void) {
//  char *str = "Hello World\r\n";
//  HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
// }
