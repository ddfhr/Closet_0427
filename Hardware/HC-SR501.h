#ifndef __BODY_HW_H
#define __BODY_HW_H
#include "stm32f1xx_hal.h"
#include "math.h"

/***************根据自己需求更改****************/
// BODY_HW GPIO宏定义
#define BODY_HW_GPIO_PORT       GPIOA
#define BODY_HW_GPIO_PIN        GPIO_PIN_3

/*********************END**********************/

void BODY_HW_Init(void);
uint16_t BODY_HW_GetData(void);

#endif /* __BODY_HW_H */