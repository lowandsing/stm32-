//
// Created by lg on 25-7-28.
//
#include "btim.h"

#include "led.h"
TIM_HandleTypeDef g_timx_handle;
//定时器中断初始函数
void btim_timx_int_init(uint16_t psc,uint16_t arr)
{
 g_timx_handle.Instance = TIM4;
 g_timx_handle.Init.Prescaler=psc;//预分频系数
g_timx_handle.Init.Period=arr;//自动重装载值
 HAL_TIM_Base_Init(&g_timx_handle);
 HAL_TIM_Base_Start_IT(&g_timx_handle);

}
//定时器基础msp函数
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
 if(tim_baseHandle->Instance==TIM4)
 {
  __HAL_RCC_TIM4_CLK_ENABLE();//打开定期器4时钟使能
  HAL_NVIC_SetPriority(TIM4_IRQn, 1,3);
  HAL_NVIC_EnableIRQ(TIM4_IRQn);
 }
}

void TIM4_IRQHandler()
{
 HAL_TIM_IRQHandler(&g_timx_handle);
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    //实现led翻转，要回去写led0的翻转
}