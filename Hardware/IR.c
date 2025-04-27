// IR.c
#include "IR.h"


void IR_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    // Configure digital input pin
    GPIO_InitStruct.Pin = IR_DO_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IR_DO_GPIO_PORT, &GPIO_InitStruct);
}

uint16_t IR_FireData(void)
{
    return !HAL_GPIO_ReadPin(IR_DO_GPIO_PORT, IR_DO_GPIO_PIN);
}