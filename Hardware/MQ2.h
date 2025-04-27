#ifndef __MQ2_H
#define __MQ2_H

#include "stm32f1xx_hal.h"

/*****************MQ-2烟雾传感器（数字DO模式）******************
 * 版本：V1.1（HAL库版本）
 * 功能：通过数字输出（DO）检测烟雾
 * 引脚：PA1（可自定义）
 **************************************************************/

// 硬件配置（根据实际连接修改）
#define MQ2_DO_GPIO_PORT        GPIOB
#define MQ2_DO_GPIO_PIN         GPIO_PIN_15

// 函数声明
void MQ2_Init(void);
uint8_t MQ2_GetData(void);  // 返回0/1表示烟雾状态

#endif /* __MQ2_H */