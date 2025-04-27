#include "main.h"
#include "stm32F1xx_hal_tim.h"
#include "tim.h"
#include "L9110.h"

void L9110_Init(){
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    // 初始化 TIM3 句柄（注意变量名从 htim1 改为 htim3）
    TIM_HandleTypeDef htim3;  // 需确保 htim3 在外部已定义或此处声明
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 72 - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 1000 - 1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.RepetitionCounter = 0;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    // 初始化 TIM3 基础配置
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
        Error_Handler();
    }

    // 配置时钟源
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }

    // 初始化 PWM
    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
        Error_Handler();
    }

    // 主从模式配置
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }

    // 配置 PWM 通道 1（TIM3 通道 1）
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    // 应用通道 1 配置
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {  // 确保是 TIM3 通道 1
        Error_Handler();
    }

    // 注意：TIM3 通道 4 的配置保留（如需删除其他通道请注释）
    // if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK) {
    //     Error_Handler();
    // }

//    // 死区时间配置（TIM3 是通用定时器，可能不支持此功能，需确认是否需要删除）
//    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
//    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
//    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
//    sBreakDeadTimeConfig.DeadTime = 0;
//    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
//    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
//    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;

//    // 如果 TIM3 不支持死区配置，需注释以下代码
//    if (HAL_TIMEx_ConfigBreakDeadTime(&htim3, &sBreakDeadTimeConfig) != HAL_OK) {
//        Error_Handler();
//    }

    // 初始化 GPIO 复用功能（需根据实际硬件连接配置）
    HAL_TIM_MspPostInit(&htim3);
}
void revolve(){
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // A-1B = LOW
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1000);   // 占空比100%
	HAL_Delay(2000);
}



void stop()  {  // 停止
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_Delay(1000);
}

void in_revolve(){  // 反转（半速）
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);   // A-1B = HIGH
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);    // 占空比50%
    HAL_Delay(2000);
}
