#ifndef _pwm_H
#define _pwm_H

#include "sys.h"

void TIM3_CH12_steer_PWM_Init(u16 per,u16 psc);
void steerFrequency_Init(u8 fre);
void set_steerDuty(u8 index, u16 duty);

#endif
