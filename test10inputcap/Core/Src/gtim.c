//
// Created by lg on 25-8-16.
//
#include  "gtim.h"
TIM_HandleTypeDef g_timx_cap_chy_handle;
//定时器输入捕获初始化配置
void gtimx_timx_cap_chy_init(uint16_t arr, uint16_t psc)
{
    TIM_IC_InitTypeDef timx_ic_cap_chy = {0};
    g_timx_cap_chy_handle.Instance = TIM5; //定时器5
    g_timx_cap_chy_handle.Init.Prescaler = psc; //分频
    g_timx_cap_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP; //计数模式：递增
    g_timx_cap_chy_handle.Init.Period = arr; //重装载值
    HAL_TIM_IC_Init(&g_timx_cap_chy_handle);;

    timx_ic_cap_chy.ICPolarity = TIM_ICPOLARITY_RISING; //上升沿捕获
    timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_DIRECTTI; //捕获选择，映射到T1
    timx_ic_cap_chy.ICPrescaler = TIM_ICPSC_DIV1; //输入分频
    timx_ic_cap_chy.ICFilter = 0; //滤波器
    HAL_TIM_IC_ConfigChannel(&g_timx_cap_chy_handle, &timx_ic_cap_chy,TIM_CHANNEL_1);

    __HAL_TIM_ENABLE_IT(&g_timx_cap_chy_handle, TIM_IT_UPDATE); //使能更新中断
    HAL_TIM_IC_Start_IT(&g_timx_cap_chy_handle, TIM_CHANNEL_1); //开始输入捕获中断
}

//定时器输入捕获msp设置
void Tim_IC_msp_init(TIM_HandleTypeDef* tim_handle)
{
    if (tim_handle->Instance == TIM5) //输入通道捕获
    {
        GPIO_InitTypeDef gpio_init_struct;
        __HAL_RCC_TIM5_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        gpio_init_struct.Pin = GPIO_PIN_0;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLDOWN;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);

        HAL_NVIC_SetPriority(TIM5_IRQn, 1, 3);
        HAL_NVIC_EnableIRQ(TIM5_IRQn);
    }
}

/* 输入捕获状态(g_timxchy_cap_sta) * [7] :0,没有成功的捕获;1,成功捕获到一次. * [6] :0,还没捕获到高电平;1,已经捕获到高电平了.
 * * [5:0]:捕获高电平后溢出的次数,最多溢出63次,所以最长捕获值 = 63*65536 + 65535 = 4194303 *
 * 注意:为了通用,我们默认ARR和CCRy都是16位寄存器,
 * 对于32位的定时器(如:TIM5),也只按16位使用
 * * 按1us的计数频率,最长溢出时间为:4194303 us, 约4.19秒 * * (说明一下：正常32位定时器来说,1us计数器加1,溢出时间:4294秒) */
uint8_t g_timx_cap_chy_cap_sta = 0; //输入捕获状态
uint16_t g_timx_cap_chy_cap_val = 0; //输入捕获值
void TIM5_IRQHandler()
{
    HAL_TIM_IRQHandler(&g_timx_cap_chy_handle);
}

//定时器输入捕获中断回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef* htim)
{
    // 判断是否是 TIM5 的中断
    if (htim->Instance == TIM5)
    {
        // 如果还未成功捕获一次完整脉宽（Bit7 = 0）
        if ((g_timx_cap_chy_cap_sta & 0x80) == 0)
        {
            // 如果已经捕获过上升沿（Bit6 = 1）
            if (g_timx_cap_chy_cap_sta & 0x40)
            {
                // 捕获到下降沿，完成一次高电平脉宽测量
                g_timx_cap_chy_cap_sta |= 0x80; // 设置Bit7表示捕获成功
                g_timx_cap_chy_cap_val = HAL_TIM_ReadCapturedValue(&g_timx_cap_chy_handle, TIM_CHANNEL_1); // 读取CCR1

                // 重置捕获极性，为下一次捕获上升沿做准备
                TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, TIM_CHANNEL_1);
                TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);
            }
            else
            {
                // 第一次捕获上升沿
                g_timx_cap_chy_cap_sta = 0;          // 清空状态
                g_timx_cap_chy_cap_val = 0;          // 清空捕获值
                g_timx_cap_chy_cap_sta |= 0x40;      // 标记已捕获上升沿
                __HAL_TIM_DISABLE(&g_timx_cap_chy_handle);       // 暂时关闭定时器
                __HAL_TIM_SET_COUNTER(&g_timx_cap_chy_handle, 0); // 清零计数器

                // 设置下一次捕获为下降沿
                TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, TIM_CHANNEL_1);
                TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING);
                __HAL_TIM_ENABLE(&g_timx_cap_chy_handle);        // 重新使能定时器
            }
        }
    }
}

//定时器更新中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM5)
    {
        // 如果还未成功捕获一次完整脉宽
        if ((g_timx_cap_chy_cap_sta & 0x80) == 0)
        {
            // 已经捕获到上升沿
            if (g_timx_cap_chy_cap_sta & 0x40)
            {
                // 检查溢出计数是否达到上限（6bit，最大63次）
                if ((g_timx_cap_chy_cap_sta & 0x3f) == 0x3f)
                {
                    // 高电平太长，认为捕获完成，设置CCR值为最大
                    TIM_RESET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, TIM_CHANNEL_1);
                    TIM_SET_CAPTUREPOLARITY(&g_timx_cap_chy_handle, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING);
                    g_timx_cap_chy_cap_sta |= 0x80;      // 标记捕获完成
                    g_timx_cap_chy_cap_val = 0xffff;     // 最大值，表示溢出
                }
                else
                {
                    // 累计溢出次数（Bit5~0存储溢出次数）
                    g_timx_cap_chy_cap_sta++;
                }
            }
        }
    }
}