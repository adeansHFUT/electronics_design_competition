#ifndef _bsp_pwm_H
#define _bsp_pwm_H

#include "sys.h"

void TIM3_CH12_steer_PWM_Init(u16 per,u16 psc);
void steerFrequency_Init(u8 fre);
void set_steerDuty(u8 index, u16 duty);

#endif
