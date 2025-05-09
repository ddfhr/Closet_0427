/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "DHT11.h"
#include "jsq.h"
#include "PTC.h"
#include "humi_balance.h"
#include "tem_balance.h"
#include "L9110.h"
#include "IR.H"
#include "OLED.h"
#include "MQ2.h"
#include "Beep.h"


#include "OLED.h" 
#include "HC-SR501.h"
#include "esp8266.h"
#include "main_program.h"
#include "time_handle.h"
#include "cJSON.h"
#include "time_handle.h"
#include <string.h>
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int16_t ir =2;
uint16_t mq2 = 3;
int value = 2;
uint8_t dataRcvd[10];
int asd = 0;
int rec_buffer = 256;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void irmq2()
{
	  ir = IR_FireData();
		mq2 = MQ2_GetData();  
		if(ir == 0&&mq2 == 0)
		{
			OLED_printf(2,0,"normal");
			BEEP_OFF();
		}
		else
		{
			OLED_printf(2,0,"warnin");
		  BEEP_ON();
		}
}
void Body_hc()
{
    value = BODY_HW_GetData();
	  if (value == 1)  // ��⵽����
    {
        OLED_printf(3,0,"people");
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//			 if (asd == 100)
//			 {
          send_json_data_t();
			 //}

    }
    else
    {
       OLED_printf(3,0,"nonene");
			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
//			 if (asd == 100)
//			 {
           //send_json_data_f();
			               send_json_data_t();

			// }

    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
	 HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN 2 */
	esp8266_send_cmd((uint8_t *)"AT+MQTTUSERCFG=0,1,\"" MQTT_CLIENT_ID "\",\"" MQTT_USER_NAME "\",\"" MQTT_PASSWD "\",0,0,\"\"\r\n", strlen("AT+MQTTUSERCFG=0,1,\"" MQTT_CLIENT_ID "\",\"" MQTT_USER_NAME "\",\"" MQTT_PASSWD "\",0,0,\"\"\r\n"), "OK");
	OLED_init();
  HAL_TIM_Base_Start(&htim1);
	JSQ_Init();
	PTC_Init();
	L9110_Init();
	IR_Init();
	BEEP_Init();
	BODY_HW_Init();
  humi_pid_init();
	temp_pid_init();
  UART_Start_Receive_IT(&huart1,(uint8_t *)&rec_buffer,1);
	esp8266_connect_server();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		 DHT11_Read_Data();
		 humi_balance();
		 tem_balance();
		 humi_updata_cal();
		 temp_updata_cal();
		 irmq2();
		 Body_hc();
		 OLED_printf(0,0,"temp:%d",Temperature);
		 OLED_printf(1,0,"humi:%d",Humidity );
		 OLED_refresh_gram();
     asd = asd +1;
		 if(asd == 1)
		 {
			  send_json_data_th();
			  asd = 0;
			  if (value == 1)
          send_json_data_t();
				else
          send_json_data_f();
		 }
		 if(asd == 1000)
		 {
			  send_json_data_th();
			  if (value == 1)
          send_json_data_t();
				else
          send_json_data_f();
		 }
		 
     HAL_Delay(100);
		
  }
  /* USER CODE END 3 */
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
