#include "main_program.h"
#include "time_handle.h"
#include <string.h>
#include "stdio.h"
#include "cJSON.h"
#include "esp8266.h"
#include "DHT11.h"

int set = 0;
/*
*************************************
变量或函数声明
*************************************
*/
void receive_json_data(void);
void board_sensor_collect(void);
void parse_json_data(void);
/*
*************************************
变量定义
*************************************
*/
uint8_t rec_msg[256] = {0};
uint8_t rec_len = 0;

static Timer timer_led_blink;
static Timer timer_sensor_collect;
static Timer timer_receive_msg;
static Timer timer_send_msg;

int led_state = 0;

uint8_t temp_value = 0;
uint8_t humi_value = 0;

/*
*************************************
初始化程序,开机只执行一次
*************************************
*/
void setup(void)
{
	set = 1;
	esp8266_init();

  timer_peripheral_init();

	timer_init(&timer_receive_msg, receive_json_data, 100, 1);
	//timer_init(&timer_send_msg, send_json_data, 1000, 1);
	timer_init(&timer_sensor_collect, board_sensor_collect, 1000, 1);

	timer_start(&timer_receive_msg);
	timer_start(&timer_send_msg);
	timer_start(&timer_sensor_collect);
}
/*
*************************************
主程序,一直执行
*************************************
*/
void loop(void)
{
	timer_loop();
}

void receive_json_data(void)
{

	if (esp8266_receive_msg(rec_msg, &rec_len) == TRUE)
	{
		parse_json_data();

		printf("rec_msg:%s\r\n", rec_msg);
		memset(rec_msg, 0, rec_len);
	}
}

void send_json_data_th(void)
{

	uint8_t msg[128] = {0};
  
	sprintf((char *)msg, "{\\\"temperature\\\":%d\\,\\\"humidity\\\":%d}", Temperature,Humidity );
	esp8266_send_msg(msg);
//	if (esp8266_send_msg(msg) == TRUE)
//	{
//		//printf("send msg %s\r\n", msg);
//		set = 1;
//	}
}

void send_json_data_t(void)
{

	uint8_t msg[128] = {0};
  
	sprintf((char *)msg, "{\\\"switch\\\":true}" );
	esp8266_send_msg(msg);
//	if (esp8266_send_msg(msg) == TRUE)
//	{
//		//printf("send msg %s\r\n", msg);
//		set = 1;
//	}
}


void send_json_data_f(void)
{

	uint8_t msg[128] = {0};
  
	sprintf((char *)msg, "{\\\"switch\\\":false}");
	esp8266_send_msg(msg);
//	if (esp8266_send_msg(msg) == TRUE)
//	{
//		//printf("send msg %s\r\n", msg);
//		set = 1;
//	}
}

void board_sensor_collect(void)
{

}

void parse_json_data(void)
{

	cJSON *json, *json_led;

	json = cJSON_Parse((const char *)rec_msg);
	if (NULL == json)
	{
		printf("JSON Error before: [%s]\n", cJSON_GetErrorPtr());
	}
	json_led = cJSON_GetObjectItem(json, "led");
	if (json_led->type == cJSON_Number)
	{
		led_state = json_led->valueint;
	}
	cJSON_Delete(json);
}
