#include "main.h"
#include "stm32F1xx_hal_tim.h"
#include "tim.h"
#include "L9110.h"

void L9110_Init(){
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    // ��ʼ�� TIM3 �����ע��������� htim1 ��Ϊ htim3��
    TIM_HandleTypeDef htim3;  // ��ȷ�� htim3 ���ⲿ�Ѷ����˴�����
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 72 - 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 1000 - 1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.RepetitionCounter = 0;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    // ��ʼ�� TIM3 ��������
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
        Error_Handler();
    }

    // ����ʱ��Դ
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
        Error_Handler();
    }

    // ��ʼ�� PWM
    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
        Error_Handler();
    }

    // ����ģʽ����
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }

    // ���� PWM ͨ�� 1��TIM3 ͨ�� 1��
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    // Ӧ��ͨ�� 1 ����
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {  // ȷ���� TIM3 ͨ�� 1
        Error_Handler();
    }

    // ע�⣺TIM3 ͨ�� 4 �����ñ���������ɾ������ͨ����ע�ͣ�
    // if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK) {
    //     Error_Handler();
    // }

//    // ����ʱ�����ã�TIM3 ��ͨ�ö�ʱ�������ܲ�֧�ִ˹��ܣ���ȷ���Ƿ���Ҫɾ����
//    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
//    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
//    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
//    sBreakDeadTimeConfig.DeadTime = 0;
//    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
//    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
//    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;

//    // ��� TIM3 ��֧���������ã���ע�����´���
//    if (HAL_TIMEx_ConfigBreakDeadTime(&htim3, &sBreakDeadTimeConfig) != HAL_OK) {
//        Error_Handler();
//    }

    // ��ʼ�� GPIO ���ù��ܣ������ʵ��Ӳ���������ã�
    HAL_TIM_MspPostInit(&htim3);
}
void revolve(){
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // A-1B = LOW
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1000);   // ռ�ձ�100%
	HAL_Delay(2000);
}



void stop()  {  // ֹͣ
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_Delay(1000);
}

void in_revolve(){  // ��ת�����٣�
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);   // A-1B = HIGH
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);    // ռ�ձ�50%
    HAL_Delay(2000);
}
