//
// Created by lg on 25-7-6.
//
#include "stm32f1xx.h"
#include "led.h"

/*初始化led， 正点原子ZET6开发板*/
void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};  // 清零结构体

    // 使能 GPIO 时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // 配置 PB5 引脚
    GPIO_InitStructure.Pin = GPIO_PIN_5;  // 选择  引脚
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  // 设置为推挽输出模式
    GPIO_InitStructure.Pull = GPIO_NOPULL;  // 无上下拉电阻
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;  // 输出速度低

    // 初始化 GPIO，引脚设置
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);


    // 设置  为高电平（如果 LED 是低电平点亮则使用 RESET）
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

}


/*低电平点亮*/
void  led_on(void)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
}
void  led_off(void)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
}
void  led_toggle(void)
{
    HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
}

