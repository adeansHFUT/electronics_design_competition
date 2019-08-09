#ifndef _bsp_led_H
#define _bsp_led_H

#include "stm32f10x.h"

/*  LED时钟端口、引脚定义 */
#define LED_PORT 			GPIOC   
#define LED_PIN 			GPIO_Pin_7
#define LED_PORT_RCC		RCC_APB2Periph_GPIOC

	
void LED_Init(void);
void LED_Turn0(void);
void LED_Turn1(void);
void Fire_Init();
void Charge_Init();
	
#endif
