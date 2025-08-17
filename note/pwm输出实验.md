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



现象
教程是通过示波器展示了输出pwm波形的

## 什么是占空比（Duty Cycle）

- 直白说法：高电平在整个波形中的占比
- 公式：占空比 = 高电平时间 / 周期总时间 × 100%
- 常见情况：
  - 100%：一直高电平，LED 全亮
  - 0%：一直低电平，LED 熄灭
  - 50%：一半时间亮，一半时间灭 → 看起来是中亮度
- 实际使用：控制 LED 呼吸灯、调速电机、调光等
图像上CCRx以下是代表低电平，ARR代表全段，所以CCRx以上是代表高电平，所以CCRx以上/ARR就为占空比



