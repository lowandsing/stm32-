#include "usart.h"

#include <string.h>

#include "stm32f1xx_hal_uart.h"
#include "stm32f1xx_hal_gpio.h"

UART_HandleTypeDef g_usart_handle;
uint8_t g_rx_buffer[1]; //缓存
uint8_t g_uart_1_rx_flag = 0; //串口接收到数据标志
/*usart初始化*/
void uart_Init(uint32_t baudrate)//参数自己传入波特率，这个函数会进行一些预处理，重置寄存器之类的
{
  g_usart_handle.Instance = USART1;//选择usart寄存器基地址
    g_usart_handle.Init.BaudRate = baudrate;//波特率
    g_usart_handle.Init.WordLength = UART_WORDLENGTH_8B;//数据位长度
    g_usart_handle.Init.StopBits = UART_STOPBITS_1;//停止位
    g_usart_handle.Init.Parity = UART_PARITY_NONE; //校验位
    g_usart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;//硬件流控制
    g_usart_handle.Init.Mode = UART_MODE_TX_RX;//读写
    HAL_UART_Init(&g_usart_handle);//传入结构体基地址
    // 启用中断接收
    HAL_UART_Receive_IT(&g_usart_handle, g_rx_buffer, 1);
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if (huart->Instance == USART1)
    {
        __HAL_RCC_USART1_CLK_ENABLE(); //usart1时钟使能
        __HAL_RCC_GPIOA_CLK_ENABLE(); //串口gpioa使能
        /*TX*/
        GPIO_InitStructure.Pin = GPIO_PIN_9; // 选择 PA9 引脚
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP; // 设置为推挽复用模式
        GPIO_InitStructure.Pull = GPIO_NOPULL; // 无上下拉电阻
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 输出速度高
        // 初始化 GPIOA，引脚设置
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
        //复用RX
        GPIO_InitStructure.Pin = GPIO_PIN_10; // 选择 PA10接收 引脚
        GPIO_InitStructure.Mode = GPIO_MODE_AF_INPUT; // 设置为推挽复用模式
        GPIO_InitStructure.Pull = GPIO_PULLUP; // 默认高电平
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 输出速度高
        HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

        HAL_NVIC_SetPriority(USART1_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
    }
}
//串口1中断服务函数
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&g_usart_handle); //会清除中断

}
//串口数据传输接收完成回调函
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        g_uart_1_rx_flag = 1;

        // 再次启动中断接收，准备下一次接收
        HAL_UART_Receive_IT(&g_usart_handle, g_rx_buffer, 1);
    }
}
