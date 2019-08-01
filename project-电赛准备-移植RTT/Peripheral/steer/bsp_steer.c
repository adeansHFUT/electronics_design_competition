#include "bsp_steer.h"
#include "bsp_pwm.h"
#include "PORT_cfg.h"

My_pwm_device steer1, steer2;


/*******************************************************************************
* 函 数 名         : bsp_steer_init
* 函数功能		   : 所有舵机设备初始化
* 输    入         : 频率.1-10（单位是kMz）
* 输    出         : 无
*******************************************************************************/
void bsp_steer_init(u8 fre)
{
	steer1.pwm_port = steer1_port;
	steer1.pwm_port_RCC = steer1_port_RCC;
	steer1.pwm_pin = steer1_pin;
	steer1.timx = steer1_timx;
	steer1.tim_channel = steer1_TIM_channel;
	steer2.pwm_port = steer2_port;
	steer2.pwm_port_RCC = steer2_port_RCC;
	steer2.pwm_pin = steer2_pin;
	steer2.timx = steer2_timx;
	steer2.tim_channel = steer2_TIM_channel;
	//steer1.pwm_arr = 1000;
	pwm_Frequency_Init(steer1, fre);   // 同一个tim下pwm的频率必须相同
	pwm_Frequency_Init(steer2, fre);
}
