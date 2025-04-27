#ifndef INC_JSQ_H_
#define INC_JSQ_H_

#include "stm32f1xx_hal.h"

#define JSQ_ON() 		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)
#define JSQ_OFF() 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)

void JSQ_Init(void);

#endif /* INC_JSQ_H_ */


