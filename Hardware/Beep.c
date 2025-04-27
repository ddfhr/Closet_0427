#include "Beep.h"

/*****************蜂鸣器驱动实现（HAL库）******************/
void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能GPIO时钟（已在CubeMX中自动生成，此处可省略）
    // __HAL_RCC_GPIOA_CLK_ENABLE();

    // 配置GPIO
    GPIO_InitStruct.Pin = BEEP_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // 推挽输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;          // 无上下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速模式
    HAL_GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStruct);

    // 默认关闭蜂鸣器
    BEEP_OFF();
}