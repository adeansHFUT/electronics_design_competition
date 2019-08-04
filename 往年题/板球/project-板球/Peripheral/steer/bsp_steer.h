#ifndef _bsp_steer_H
#define _bsp_steer_H
#include "sys.h"
#include "bsp_pwm.h"


extern My_pwm_device steer1, steer2;
void bsp_steer_init(uint16_t fre);
#endif