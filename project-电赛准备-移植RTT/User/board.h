#ifndef __board_H__
#define __board_H__

#include "stm32f10x.h"
/*������bspͷ�ļ�*/
#include "bsp_led.h" // led
#include "bsp_oled.h"
#include "sys.h"
#include "SysTick.h" // ����delay����,����os������
#include "string.h"
#include "bsp_time.h"
#include "bsp_pwm.h"
#include "bsp_key.h"
#include "bsp_usart.h"
/**********��������************/
void rt_hw_board_init(void);
void SysTick_Handler(void);
#endif