#ifndef _LED_H
#define _LED_H

#include "stm32f1xx_hal.h""
/**
 * @file led.h
 * @brief LED 灯控制头文件
 *
 * 该文件定义了 LED 引脚、时钟使能、控制宏和初始化函数。
 * 用于快速操作开发板上的 LED 灯。
 */

/*------------------------- 引脚定义 -------------------------*/
/**
 * @brief LED0 引脚定义
 * LED0 连接到 GPIOB 的 PIN5
 */
#define LED0_GPIO_PORT                  GPIOB
#define LED0_GPIO_PIN                   GPIO_PIN_5
#define LED0_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)  // 使能 GPIOB 时钟

/**
 * @brief LED1 引脚定义
 * LED1 连接到 GPIOE 的 PIN5
 */
#define LED1_GPIO_PORT                  GPIOE
#define LED1_GPIO_PIN                   GPIO_PIN_5
#define LED1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)  // 使能 GPIOE 时钟


/*------------------------- LED 开关宏 -------------------------*/
/**
 * @brief 控制 LED0 开关
 * @param x: 1=点亮LED0，0=熄灭LED0
 * 使用 HAL_GPIO_WritePin 控制引脚电平
 */
#define LED0(x)   do{ x ? \
HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_SET) : \
HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_RESET); \
}while(0)

/**
 * @brief 控制 LED1 开关
 * @param x: 1=点亮LED1，0=熄灭LED1
 */
#define LED1(x)   do{ x ? \
HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET) : \
HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET); \
}while(0)


/*------------------------- LED 翻转宏 -------------------------*/
/**
 * @brief 翻转 LED0 状态
 * 如果 LED0 亮 → 熄灭；如果 LED0 灭 → 点亮
 */
#define LED0_TOGGLE()   do{ HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_GPIO_PIN); }while(0)

/**
 * @brief 翻转 LED1 状态
 */
#define LED1_TOGGLE()   do{ HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN); }while(0)


/*------------------------- 函数声明 -------------------------*/
/**
 * @brief 初始化 LED 引脚
 * 配置 GPIO 模式为输出并关闭 LED 灯
 */
void led_init(void);

#endif
