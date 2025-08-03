//
// Created by lg on 25-8-3.
//
#include "gtim.h"

TIM_HandleTypeDef gtim_timx_pwm_chy_handle;
void gtim_timx_pwm_chy_init(uint16_t psc,uint16_t arr)
{
  TIM_OC_InitTypeDef timx_oc_pwm_chy;
  gtim_timx_pwm_chy_handle.Instance = TIM3;//寄存器基地址
  gtim_timx_pwm_chy_handle.Init.Prescaler = psc;//分频系数
  gtim_timx_pwm_chy_handle.Init.Period = arr;//重装载值
  gtim_timx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;//计数模式，例如向上计数模式
  HAL_TIM_PWM_Init(&gtim_timx_pwm_chy_handle);

  timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;
  timx_oc_pwm_chy.Pulse = arr/2;   //CCRx的值
  timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;  //设置极性为低
  HAL_TIM_PWM_ConfigChannel(&gtim_timx_pwm_chy_handle,&timx_oc_pwm_chy,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&gtim_timx_pwm_chy_handle,TIM_CHANNEL_2);

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
  if(htim_pwm->Instance==TIM3)
  {
    GPIO_InitTypeDef GPIO_InitStructure = {0};  // 清零结构体

    // 使能  时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();

    // 配置GPIO PB5 引脚
    GPIO_InitStructure.Pin = GPIO_PIN_5;  // 选择  引脚
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;  // 要设置成复用，
    GPIO_InitStructure.Pull = GPIO_NOPULL;  // 无上下拉电阻
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;  // 输出速度低
    // 初始化 GPIO，引脚设置
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    __HAL_RCC_AFIO_CLK_ENABLE(); //io口复用，之前学外部中断也用到这个
    __HAL_AFIO_REMAP_TIM3_PARTIAL();




  }

}