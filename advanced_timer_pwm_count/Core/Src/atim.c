//
// Created by lg on 25-9-13.
//
#include "atim.h"

TIM_HandleTypeDef g_timx_npwm_chy_handle; //定时器句柄
static uint8_t g_npwm_remian =0;
/*高级定时器timx  通道y输出指定个数pwm初始化函数*/
/**
 * @brief  初始化高级定时器 TIMx 的 PWM 输出（单通道），并开启更新中断
 * @param  arr: 自动重装载值（决定 PWM 周期 = (arr+1)/计数时钟频率）
 * @param  psc: 预分频器值（决定计数时钟频率 = 定时器时钟 / (psc+1)）
 * @note   这个函数的作用就是：配置 TIM8 通道 1 输出 PWM 波形，
 *         占空比默认为 50%，并且开启更新中断。
 */
void atim_timx_npwm_chy_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_npwm_chy = {0};  // 配置 PWM 通道的结构体，先清零

    // 1. 配置定时器基本参数
    g_timx_npwm_chy_handle.Instance = TIM8;           // 选择高级定时器 TIM8
    g_timx_npwm_chy_handle.Init.Prescaler = psc;      // 预分频器：降低计数时钟频率
    g_timx_npwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP; // 向上计数模式
    g_timx_npwm_chy_handle.Init.Period = arr;         // 自动重装载值，决定 PWM 周期
    g_timx_npwm_chy_handle.Init.ClockDivision = 0;    // 时钟分频，通常设 0
    HAL_TIM_PWM_Init(&g_timx_npwm_chy_handle);        // 初始化 PWM 模式

    // 2. 配置 PWM 输出通道
    timx_oc_npwm_chy.OCMode = TIM_OCMODE_PWM1;        // 设置为 PWM1 模式
    timx_oc_npwm_chy.Pulse = arr / 2;                 // 比较值 = 周期一半 → 占空比 50%
    timx_oc_npwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;// 输出极性：高电平有效
    HAL_TIM_PWM_ConfigChannel(&g_timx_npwm_chy_handle,
                              &timx_oc_npwm_chy,
                              TIM_CHANNEL_1);         // 配置通道 1 的输出比较参数

    // 3. 使能定时器更新中断
    __HAL_TIM_ENABLE_IT(&g_timx_npwm_chy_handle, TIM_IT_UPDATE);

    // 4. 启动 PWM 输出
    HAL_TIM_PWM_Start(&g_timx_npwm_chy_handle, TIM_CHANNEL_1);
}

/**
 * @brief  TIM8 PWM 的底层硬件初始化函数（由 HAL_TIM_PWM_Init() 自动调用）
 * @param  htim: 定时器句柄指针
 * @note   主要完成以下工作：
 *         1. 使能 TIM8 和 GPIOC 的时钟
 *         2. 配置 TIM8_CH1 的引脚复用功能
 *         3. 配置 TIM8 的更新中断优先级并使能
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim)
{
    if(htim->Instance == TIM8)  // 判断当前初始化的定时器是不是 TIM8
    {
        GPIO_InitTypeDef gpio_init_struct;

        /* 1. 使能 TIM8 外设时钟 */
        __HAL_RCC_TIM8_CLK_ENABLE();

        /* 2. 使能 GPIOC 时钟，因为 TIM8_CH1 在 PC6 */
        __HAL_RCC_GPIOC_CLK_ENABLE();

        /* 3. 配置 PC6 为复用推挽输出，用于输出 TIM8_CH1 PWM */
        gpio_init_struct.Pin   = GPIO_PIN_6;           // 选择 PC6 引脚
        gpio_init_struct.Mode  = GPIO_MODE_AF_PP;      // 复用推挽功能
        gpio_init_struct.Pull  = GPIO_PULLUP;          // 上拉电阻
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速输出模式
        HAL_GPIO_Init(GPIOC, &gpio_init_struct);       // 初始化 GPIOC 的 PC6

        /* 4. 配置 TIM8 更新中断的优先级并使能 NVIC */
        HAL_NVIC_SetPriority(TIM8_UP_IRQn, 1, 3);  // 设置优先级：抢占优先级=1，子优先级=3
        HAL_NVIC_EnableIRQ(TIM8_UP_IRQn);          // 使能 TIM8 更新中断
    }
}
//设置pwm个数函数
/**
 * @brief  设置 TIM8 PWM 输出指定个数脉冲
 * @param  npwm: 需要输出的 PWM 脉冲个数（1~255）
 * @note   该函数会启动定时器，使定时器开始生成 npwm 个 PWM 周期。
 */
void atim_timx_npwm_chy_set(uint8_t npwm)
{
    if (npwm == 0)  // 如果要输出的脉冲数为 0，则直接返回，不做任何操作
    {
        return;
    }

    g_npwm_remian = npwm; // 保存剩余要输出的 PWM 个数

    // 立即触发一次更新事件，让定时器立即开始计数
    HAL_TIM_GenerateEvent(&g_timx_npwm_chy_handle, TIM_EVENTSOURCE_UPDATE);

    // 使能 TIM8 计数器，开始 PWM 输出
    __HAL_TIM_ENABLE(&g_timx_npwm_chy_handle);
}

/**
 * @brief  TIM8 更新中断处理函数
 * @note   当 TIM8 溢出（更新事件）时进入此中断，由 HAL 库调用中断回调
 */
void TIM8_UP_IRQHandler(void)
{
    // 调用 HAL 库的通用中断处理函数，内部会判断是更新中断并调用回调
    HAL_TIM_IRQHandler(&g_timx_npwm_chy_handle);
}

/**
 * @brief  TIM8 更新事件回调函数（周期溢出回调）
 * @param  htim: 当前触发回调的定时器句柄
 * @note   当定时器每次产生更新事件（溢出）时会调用此函数
 *         用于控制输出指定个数的 PWM 脉冲
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM8)  // 判断是 TIM8 的更新事件
    {
        if (g_npwm_remian)  // 如果还有剩余的 PWM 脉冲未输出
        {
            TIM8->CCR1 = g_npwm_remian - 1; // 设置比较寄存器，控制输出剩余脉冲数
            // 立即触发更新事件，保证计数器重新开始
            HAL_TIM_GenerateEvent(&g_timx_npwm_chy_handle, TIM_EVENTSOURCE_UPDATE);
            __HAL_TIM_ENABLE(&g_timx_npwm_chy_handle); // 重新使能计数器
            g_npwm_remian = 0; // 已经安排好剩余脉冲，重置计数
        }
        else  // 如果没有剩余脉冲需要输出
        {
            TIM8->CR1 &= ~(1 << 0); // 清除 CR1 的 CEN 位，停止计数器，PWM 输出停止
        }
    }
}



