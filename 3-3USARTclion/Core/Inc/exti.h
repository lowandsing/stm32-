//
// Created by lg on 25-7-11.
//

#ifndef EXTI_H
#define EXTI_H
void exti_init(void);
void EXTI1_IRQHandler(void);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
#endif //EXTI_H
