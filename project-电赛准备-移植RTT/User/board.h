#ifndef __board_H__
#define __board_H__

#include "stm32f10x.h"
/*������bspͷ�ļ�*/
#include "led.h" // led
#include "oled.h"
#include "sys.h"
#include "SysTick.h" // ����delay����,����os������
#include "string.h"
#include "time.h"
#include "pwm.h"
#include "key.h"
#include "bsp_usart.h"
/**********��������************/
void rt_hw_board_init(void);
void SysTick_Handler(void);
#endif