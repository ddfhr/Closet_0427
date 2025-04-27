#include "MQ2.h"

/*****************MQ-2初始化******************/
void MQ2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能GPIO时钟（CubeMX已自动生成，此处可省略）
    // __HAL_RCC_GPIOA_CLK_ENABLE();

    // 配置GPIO为上拉输入
    GPIO_InitStruct.Pin = MQ2_DO_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;  // 上拉电阻避免悬空
    HAL_GPIO_Init(MQ2_DO_GPIO_PORT, &GPIO_InitStruct);
}

/*****************读取烟雾状态******************/
uint8_t MQ2_GetData(void)
{
    // DO引脚低电平表示检测到烟雾（注意逻辑取反）
    return (HAL_GPIO_ReadPin(MQ2_DO_GPIO_PORT, MQ2_DO_GPIO_PIN) == GPIO_PIN_RESET) ? 1 : 0;
}