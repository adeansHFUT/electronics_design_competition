#ifndef _bsp_pwm_H
#define _bsp_pwm_H

#include "sys.h"
#include "PORT_cfg.h"
/*pwm设备*/
typedef struct{
	GPIO_TypeDef* pwm_port;
	uint32_t pwm_port_RCC;
	uint16_t pwm_pin;
	TIM_TypeDef* timx;
	uint32_t timx_RCC;
	uint16_t tim_channel;
	uint16_t pwm_ccr;  // 保存捕获比较寄存器的值
	uint16_t pwm_arr;  // 保存自动重装寄存器的值
}My_pwm_device;  


void TIM3_CH12_steer_PWM_Init(u16 per,u16 psc);
void PWM_Init(My_pwm_device pwm_device, u16 psc); //改良版
void steerFrequency_Init(u8 fre);
void pwm_Frequency_Init(My_pwm_device pwm_device, u8 fre);
void set_steerDuty(u8 index, u16 duty);
void pwm_set_Duty(My_pwm_device pwm_device, uint16_t duty);
#endif
