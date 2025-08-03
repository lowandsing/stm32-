# include "led.h"
#include "../STM32F1xx_HAL_Driver/Inc/stm32f1xx_hal.h"
void led_init(){
	GPIO_InitTypeDef gpio_init_struct;
	/*开启时钟*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
	gpio_init_struct.Pin=GPIO_PIN_0;
	gpio_init_struct.Mode=GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Speed=GPIO_SPEED_FREQ_LOW;
	/*初始化gpio*/
	HAL_GPIO_Init(GPIOA , &gpio_init_struct);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

}