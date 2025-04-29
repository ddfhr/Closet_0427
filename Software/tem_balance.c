#include "tem_balance.h"
#include "pid.h"
#include "DHT11.h"
#include "PTC.h"
#include "L9110.h"

pid_t temp_location_pid;

float temp_target = 0;
float temp_now = 0;
float out_put_t = 0;

void temp_pid_init()
{
	pid_set(&temp_location_pid, 1.0f, 0.0f,0.0f, 8000.0f, 0.01f);
	
	temp_target = 23;
}

void temp_updata_cal()
{
	temp_now = Temperature;
	out_put_t=pid_cal(&temp_location_pid,temp_now, temp_target);
}
void tem_balance()
{
  if (out_put_t<0)
	{
		   PTC_OFF();
		   revolve();

	}
	else
	{
		   PTC_ON();	   
		   stop();
	}
}