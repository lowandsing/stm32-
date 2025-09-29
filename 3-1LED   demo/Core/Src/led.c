#include "led.h"


void led_init(void)
{
   __HAL_RCC_GPIOE_CLK_ENABLE();
   GPIO_InitTypeDef GPIO_InitStruct = {0};
   GPIO_InitStruct.Pin = GPIO_PIN_5;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;


   HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}


void led_toggle(void)
{
   HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_5);
   HAL_Delay(500);
}


