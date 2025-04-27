#ifndef INC_PTC_H_
#define INC_PTC_H_

#include "stm32f1xx_hal.h"

#define PTC_ON() 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
#define PTC_OFF() 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)

void PTC_Init(void);

#endif /* INC_JSQ_H_ */


