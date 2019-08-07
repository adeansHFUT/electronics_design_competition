#ifndef __board_H__
#define __board_H__

#include "stm32f10x.h"
/*开发板bsp头文件*/
#include "bsp_led.h" // led
#include "bsp_oled.h"
#include "sys.h"
#include "SysTick.h" // 包含delay函数,加了os后不能用
#include "string.h"
#include "bsp_time.h"
#include "bsp_pwm.h"
#include "bsp_key.h"
#include "bsp_usart.h"
#include "bsp_iic.h"
#include "bsp_24cxx.h"
#include "bsp_steer.h"
#include "pos_pid_control.h"
#include "inc_pid_control.h"
/**********函数声明************/
void rt_hw_board_init(void);
void SysTick_Handler(void);
void rt_hw_us_delay(rt_uint32_t us);
#endif