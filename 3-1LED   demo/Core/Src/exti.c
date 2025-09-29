// //
// // Created by lg on 25-7-11.
// //
// /*这里中断的原理是设置按键电平变化产生中断，要写监控按键电平*/
// #include "stm32f1xx.h"
// #include "led.h"
// void exti_init(void)
// {
//     GPIO_InitTypeDef GPIO_InitStructure = {0}; // 清零结构体
//
//     // 使能 GPIO 时钟
//     __HAL_RCC_GPIOB_CLK_ENABLE();
//     __HAL_RCC_AFIO_CLK_ENABLE();
//     // 配置 PB1 引脚 ，按键接在PB1
//     GPIO_InitStructure.Pin = GPIO_PIN_1; // 选择 PB1 引脚
//
//     GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING; // 设置为上下降沿输出模式
//     GPIO_InitStructure.Pull = GPIO_PULLUP; // 上拉
//     // 初始化 GPIOB，引脚设置
//     HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
//     AFIO->EXTICR[0] &= ~(0xF << (4 * 1)); // 清除   也就是1111偏移4位 ,1111 00000,再取反0000 1111
//     AFIO->EXTICR[0] |= (0x1 << (4 * 1)); // EXTI1 → PB1
//     HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0); //设置优先级，选择exti线1，抢占0，响应0
//     HAL_NVIC_EnableIRQ(EXTI1_IRQn); //中断使能
// }
//
// void EXTI1_IRQHandler(void)
// {
//      HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
//
// }
//
// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
// {
//     for (volatile int i = 0; i < 10000; i++); // 简单消抖
//     if (GPIO_Pin == GPIO_PIN_1)
//     {
//         for (volatile int i = 0; i < 10000; i++); // 简单消抖
//         if ((int)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
//         {
//             // 直接点亮LED，确认中断响应
//             led_on();
//
//         }else
//         {
//             led_off();
//         }
//     }
// }
