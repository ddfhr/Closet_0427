#ifndef __DHT11_H__
#define __DHT11_H__
#include "main.h"
#include "gpio.h"
#include "tim.h"

#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_PIN_5
extern float Temperature;
extern float Humidity;

void delay(uint16_t time);
void Set_Pin_Input(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
void Set_Pin_Input(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
uint8_t Check_Response (void);
void DHT11_Start (void);
uint8_t DHT11_Read (void);
void DHT11_Read_Data(void);




#endif 
