#ifndef _bsp_steer_H
#define _bsp_steer_H
#include "sys.h"
#include "bsp_pwm.h"

#define Steer1_S3010_mid  500        // 百分之50占空比
#define Steer1_S3010_max  750
#define Steer1_S3010_min  250
#define Steer2_S3010_mid  500
#define Steer2_S3010_max  750
#define Steer2_S3010_min  250
extern My_pwm_device steer1, steer2;
void bsp_steer_init(u8 fre);
#endif