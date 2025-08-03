//
// Created by lg on 25-7-6.
//
#include "stm32f1xx.h"
#include "key.h"
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};  // 清零结构体

    // 使能 GPIOB 时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // 配置 PAb 引脚
    GPIO_InitStructure.Pin = GPIO_PIN_1;  // 选择 Pb0引脚
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  // 设置为输入模式
    GPIO_InitStructure.Pull = GPIO_PULLUP;  // 上拉电阻
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    // 初始化 GPIOB，引脚设置
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t key_scan(void)
{
    // 检测按键是否按下（低电平）
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0)
    {
        HAL_Delay(20);  // 稍微延时 20ms，增加消抖时间
        // 再次确认按键是否仍处于低电平（稳定）
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0)
        {
            // 等待按键松开
            while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0);
            return 1;  // 返回按键按下信号
        }
    }
    return 0;  // 如果按键没有按下，返回 0
}