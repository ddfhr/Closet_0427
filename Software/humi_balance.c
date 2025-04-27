#include "humi_balance.h"
#include "pid.h"
#include "DHT11.h"
#include "jsq.h"

//pid_t humi_location_speed_pid;
pid_t humi_location_pid;

float humi_target = 0;
float humi_now = 0;
float out_put = 0;

void humi_pid_init()
{
	pid_set(&humi_location_pid, 1.0f, 0.0f,0.0f, 8000.0f, 0.01f);
	
	humi_target = 40;
}

void humi_updata_cal()
{
	humi_now = Humidity;
	out_put=pid_cal(&humi_location_pid,humi_now, humi_target);
}
void humi_balance()
{
   if ( out_put >= 0)
			JSQ_ON();
		else
			JSQ_OFF();
}
