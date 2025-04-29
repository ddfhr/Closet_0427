#include "esp8266.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

int a = 0;
int b = 0;
int c = 0;
int d = 0;
int e = 0;
int f = 0;
int g = 0;
int h = 0;
int r = 0;
int j = 0;
int k = 0;
  unsigned int count = 0;
/*
**********************************************************************************************************
变量声明
**********************************************************************************************************
*/

/*
**********************************************************************************************************
变量定义
**********************************************************************************************************
*/
uint8_t uart1_data;        // 串口接收缓存数1
uint8_t uart1_buf[256];    // 串口接收缓存数组
uint16_t uart1_start = 0;  // 串口接收开始标志位
uint16_t uart1_count = 0;  // 串口接收数据计数器
uint16_t uart1_finish = 0; // 串口接收结束标志位
/*
**********************************************************************************************************
串口接收回调
**********************************************************************************************************
*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	k = 2;
  if (huart->Instance == USART1)
  {
    uart1_buf[uart1_count++] = uart1_data;
    HAL_UART_Receive_IT(&huart1, &uart1_data, 1);
    uart1_start = 1;  // 串口接收标志位置1
    uart1_finish = 0; // 串口接收数据完成标志位清零
		k = 1;
  }
}
/*
**********************************************************************************************************
串口初始化,设置esp8266的波特率为115200
**********************************************************************************************************
*/
void uart1_init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}
/*
**********************************************************************************************************
清空接收串口数组程序
**********************************************************************************************************
*/
void uart1_flag_clear(void)
{
  memset(uart1_buf, 0, (unsigned int)strlen((const char *)uart1_buf));
  uart1_count = 0;
  uart1_start = 0;
  uart1_finish = 0;
}
/*
**********************************************************************************************************
esp8266发送命令
**********************************************************************************************************
*/
unsigned char esp8266_send_cmd(unsigned char *cmd, unsigned char len, char *rec_data)
{
  unsigned char ret_val = FALSE;

  uart1_flag_clear();
  HAL_UART_Transmit(&huart1, cmd, len, 1000);
//  while ((uart1_start == 0) && (count < 1000))
//  {
//    count++;
//    HAL_Delay(1);
//  }
//  if (count >= 1000)
//  {
//    ret_val = FALSE;
//  }
//  else
//  {
//    do
//    {
//      uart1_finish++;
//      HAL_Delay(1);
//    } while (uart1_finish < 100);
//    if (strstr((const char *)uart1_buf, rec_data))
//    {
//      ret_val = TRUE;
//    }
//    else
//    {
//      ret_val = FALSE;
//    }
//  }
//  return ret_val;
}
/*
**********************************************************************************************************
esp8266配置WIFI连接网络
**********************************************************************************************************
*/
unsigned char esp8266_wifi_config(void)
{
  uint8_t ret_val = FALSE;
  uint16_t count = 0;

  uart1_flag_clear();
  HAL_UART_Transmit(&huart1, (unsigned char *)"AT+CWJAP=\"" WIFI_SSID "\",\"" WIFI_PASSWD "\"\r\n",
                    strlen("AT+CWJAP=\"" WIFI_SSID "\",\"" WIFI_PASSWD "\"\r\n"), 1000);

  while (1)
  {
    while ((uart1_start == 0) && (count < 5000))
    {
      count++;
      HAL_Delay(1);
    }
    if (count < 5000)
    {
      HAL_Delay(3000); // 返回数据不长，不需要清空buff
      if (strstr((const char *)uart1_buf, "WIFI CONNECTED"))
      {
        ret_val = TRUE;
        return ret_val;
      }
      else
      {
        ret_val = FALSE;
      }
    }
    else
    {
      ret_val = FALSE;
      return ret_val;
    }
  }
}
/*
**********************************************************************************************************
esp8266配置连接服务器
**********************************************************************************************************
*/
unsigned char esp8266_connect_server(void)
{
  uint8_t ret_val = FALSE;
  uint16_t count = 0;

  uart1_flag_clear();
  HAL_UART_Transmit(&huart1, (unsigned char *)"AT+MQTTCONN=0,\"" MQTT_BROKER_ADDRESS "\"," MQTT_BROKER_PORT ",0\r\n",
                    strlen("AT+MQTTCONN=0,\"" MQTT_BROKER_ADDRESS "\"," MQTT_BROKER_PORT ",0\r\n"), 1000);

//  while (1)
//  {
//    while ((uart1_start == 0) && (count < 5000))
//    {
//      count++;
//      HAL_Delay(1);
//    }
//    if (count < 5000)
//    {
//      HAL_Delay(1000); // 返回数据不长，不需要清空buff
//      if (strstr((const char *)uart1_buf, "OK"))
//      {
//        ret_val = TRUE;
//        return ret_val;
//      }
//      else
//      {
//        ret_val = FALSE;
//      }
//    }
//    else
//    {
//      ret_val = FALSE;
//      return ret_val;
//    }
//  }
}
/*
**********************************************************************************************************
esp8266复位
**********************************************************************************************************
*/
unsigned char esp8266_reset(void)
{
  uint8_t ret_val = FALSE;
  uint16_t count = 0;

  uart1_flag_clear();
  HAL_UART_Transmit(&huart1, (unsigned char *)"AT+RST\r\n", 8, 1000);

  while (1)
  {
    while ((uart1_start == 0) && (count < 1000))
    {
      count++;
      HAL_Delay(1);
    }
    if (count < 1000)
    {
      HAL_Delay(200);
      uart1_flag_clear(); // 清除部分杂乱的数据
      HAL_Delay(300);
      if (strstr((const char *)uart1_buf, "ready"))
      {
        ret_val = TRUE;
        return ret_val;
      }
      else
      {
        ret_val = FALSE;
      }
    }
    else
    {
      ret_val = FALSE;
      return ret_val;
    }
  }
}

/*
**********************************************************************************************************
esp8266发送消息
**********************************************************************************************************
*/
uint8_t esp8266_send_msg(uint8_t *send_msg)
{
//  uint8_t ret_val = FALSE;
//  uint16_t count = 0;
  unsigned char msg_buf[256] = {0};

  sprintf((char *)msg_buf, "AT+MQTTPUB=0,\"" PUB_TOPIC "\",\"%s\",0,0\r\n", send_msg);
  HAL_UART_Transmit(&huart1, (unsigned char *)msg_buf, strlen((const char *)msg_buf), 1000);

//  while ((uart1_start == 0) && (count < 100))
//  {
//    count++;
//    HAL_Delay(1);
//  }
//  if (count < 500)
//  {
//    HAL_Delay(50);
//    if (strstr((const char *)uart1_buf, "OK"))
//    {
//      ret_val = TRUE;
//      return ret_val;
//    }
//    else
//    {
//      ret_val = FALSE;
//      return ret_val;
//    }
//  }
//  return ret_val;
}
/*
**********************************************************************************************************
esp8266接收数据处理
**********************************************************************************************************
*/
uint8_t esp8266_receive_msg(uint8_t *rec_msg, uint8_t *rec_len)
{
  uint8_t ret_val = FALSE;
  int msg_len = 0;
  uint8_t msg_body[128] = {0};

  if (uart1_start == 1) // 指令接收标志位
  {
    // 接收到最后一个字节，再延时5ms
    do
    {
      uart1_finish++;
      HAL_Delay(1);
    } while (uart1_finish < 5); // 等待指令接收完成

    if (strstr((const char *)uart1_buf, "+MQTTSUBRECV:")) // 如果接收到的指令数组里面有这个字符串
    {
      sscanf((const char *)uart1_buf, "+MQTTSUBRECV:0,\"" SUB_TOPIC "\",%d,%s", &msg_len, msg_body);
      if (strlen((const char *)msg_body) == msg_len)
      {
        memcpy(rec_msg, msg_body, msg_len);
        *rec_len = msg_len;
        ret_val = TRUE;
      }
      else
      {
        ret_val = FALSE;
      }
    }
    else
    {
      ret_val = FALSE;
    }
  }
  else
  {
    ret_val = FALSE;
  }
  uart1_flag_clear();
  return ret_val;
}
/*
**********************************************************************************************************
esp8266初始化配置
**********************************************************************************************************
*/
void esp8266_init(void)
{
  uart1_init();
  HAL_UART_Receive_IT(&huart1, &uart1_data, 1);

 // printf("1.Setting Mode\r\n");
//  while (esp8266_send_cmd((uint8_t *)"AT+CWMODE=1\r\n", 13, "OK") != 0)
//  {
//    HAL_Delay(1000);
//  }
	a = 1;
  //printf("2.Close Echo\r\n");
//  while (esp8266_send_cmd((uint8_t *)"ATE0\r\n", 6, "OK") != 0)
//  {
//    HAL_Delay(1000);
//  }
	b = 1;
	
 // printf("3.Setting No AUTO Connect WIFI\r\n");
//  while (esp8266_send_cmd((uint8_t *)"AT+CWAUTOCONN=0\r\n", 17, "OK") != 0)
//  {
//    HAL_Delay(1000);
//  }
	
  //printf("4.Reset module\r\n");
//	c = esp8266_reset() ;
//  while (c!= 0)
//  {
//    HAL_Delay(5000);
//  }
	d = 1;
  //printf("5.Connect WIFI Config\r\n");
//  while (esp8266_wifi_config() != 0)
//  {
//    HAL_Delay(5000);
//  }
  
	e = esp8266_send_cmd((uint8_t *)"AT+MQTTUSERCFG=0,1,\"" MQTT_CLIENT_ID "\",\"" MQTT_USER_NAME "\",\"" MQTT_PASSWD "\",0,0,\"\"\r\n", strlen("AT+MQTTUSERCFG=0,1,\"" MQTT_CLIENT_ID "\",\"" MQTT_USER_NAME "\",\"" MQTT_PASSWD "\",0,0,\"\"\r\n"), "OK");
  //printf("6.Connect Service Config\r\n");
  while (esp8266_send_cmd((uint8_t *)"AT+MQTTUSERCFG=0,1,\"" MQTT_CLIENT_ID "\",\"" MQTT_USER_NAME "\",\"" MQTT_PASSWD "\",0,0,\"\"\r\n", strlen("AT+MQTTUSERCFG=0,1,\"" MQTT_CLIENT_ID "\",\"" MQTT_USER_NAME "\",\"" MQTT_PASSWD "\",0,0,\"\"\r\n"), "OK") != 0)
  {
    HAL_Delay(2000);
  }
	g = 1;
  //printf("7.Connect Service\r\n");
  while (esp8266_connect_server() != 0)
  {
    HAL_Delay(8000);
  }
	h = 1;
  //printf("8.Subscribe Topic\r\n");
  while (esp8266_send_cmd((uint8_t *)"AT+MQTTSUB=0,\"" SUB_TOPIC "\",0\r\n", strlen("AT+MQTTSUB=0,\"" SUB_TOPIC "\",0\r\n"), "OK") != 0)
  {
    HAL_Delay(2000);
  }
	r = 1;
  //printf("9.esp8266 Init success !!!\r\n");
}
