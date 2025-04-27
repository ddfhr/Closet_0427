// IR.h
#ifndef __LDR_H
#define	__LDR_H
#include "stm32f1xx_hal.h"
#include "main.h"

// Digital output mode only
#define IR_DO_GPIO_PORT     GPIOC
#define IR_DO_GPIO_PIN      GPIO_PIN_14

void IR_Init(void);
uint16_t IR_FireData(void);

#endif /* __LDR_H */