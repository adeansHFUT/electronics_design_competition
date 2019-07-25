#ifndef __board_H__
#define __board_H__

#include "stm32f10x.h"
/*开发板bsp头文件*/
#include "led.h" // led
#include "oled.h"
#include "sys.h"
#include "SysTick.h" // 包含delay函数,加了os后不能用
#include "string.h"
#include "time.h"
#include "pwm.h"
#include "key.h"
#include "bsp_usart.h"
/**********函数声明************/
void rt_hw_board_init(void);
void SysTick_Handler(void);
#endif