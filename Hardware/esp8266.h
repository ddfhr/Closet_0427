#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "main.h"

/*
*************************************
宏定义
*************************************
*/
#define TRUE (0)
#define FALSE (1)

/*
***************************************************************************************
连接的WIFI热点必须是2.4G WIFI,5GWIFI不可用
***************************************************************************************
*/
#define WIFI_SSID "test"
#define WIFI_PASSWD "12345678"
/*
***************************************************************************************

***************************************************************************************
*/
#define MQTT_CLIENT_ID "4f7d5fc0a45a4a04957270b5d23f271e"
#define MQTT_USER_NAME "vl0s0qsahtdt5vot"
#define MQTT_PASSWD "t3b1r8P0Mh"
#define MQTT_BROKER_ADDRESS "sh-1-mqtt.iot-api.com"
#define MQTT_BROKER_PORT "1883"

#define SUB_TOPIC "attributes/push"
#define PUB_TOPIC "attributes"

void esp8266_init(void);
uint8_t esp8266_receive_msg(uint8_t *rec_msg, uint8_t *rec_len);
uint8_t esp8266_send_msg(uint8_t *send_msg);
unsigned char esp8266_connect_server(void);
unsigned char esp8266_send_cmd(unsigned char *cmd, unsigned char len, char *rec_data);

#endif

/*
*************************************
使用方法：

1.配置波特率115200
2.勾选开启中断使能
3.默认GPIO是TX-PA2,RX-PA3
4.修改连接参数
5.esp8266_init();初始化
6.esp8266_receive_msg();接收数据
7.esp8266_send_msg();   发送数据,timer_init(&timer_detect_receive, esp8266_receive_msg, 100, 1);
8.uart2_receiver_handle();检查是否被调用，写在串口中断回调的靠上面,
*************************************
*/
