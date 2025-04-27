#include "HC-SR501.h"

void BODY_HW_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    /*Configure GPIO pin : PA0 */
    GPIO_InitStruct.Pin = BODY_HW_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(BODY_HW_GPIO_PORT, &GPIO_InitStruct);
}

uint16_t BODY_HW_GetData(void)
{
    return HAL_GPIO_ReadPin(BODY_HW_GPIO_PORT, BODY_HW_GPIO_PIN);
}