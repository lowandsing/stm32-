# STM32 编译报错记录

## [2025-08-03] 链接失败 - undefined reference to `HAL_TIM_PWM_Init`

**问题描述：**
构建 `gtim.c` 的 PWM 函数时报错，链接失败。

**错误信息：**
undefined reference to HAL_TIM_PWM_Init



**解决方案：**
在 `stm32f1xx_hal_conf.h` 中加上：
```c
#define HAL_TIM_MODULE_ENABLED



