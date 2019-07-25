#ifndef _led_H
#define _led_H

#include "stm32f10x.h"

/*  LEDʱ�Ӷ˿ڡ����Ŷ��� */
#define LED_PORT 			GPIOC   
#define LED_PIN 			(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)
#define LED_PORT_RCC		RCC_APB2Periph_GPIOC

	
void LED_Init(void);
void LED_Turn0(void);
void LED_Turn1(void);
	
#endif
