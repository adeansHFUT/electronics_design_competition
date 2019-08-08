/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */
#include <rthw.h>
#include <rtthread.h>
#include "board.h"
#include "include.h"
#include "key_handle.h"
#if 0
#define _SCB_BASE       (0xE000E010UL)
#define _SYSTICK_CTRL   (*(rt_uint32_t *)(_SCB_BASE + 0x0))
#define _SYSTICK_LOAD   (*(rt_uint32_t *)(_SCB_BASE + 0x4))
#define _SYSTICK_VAL    (*(rt_uint32_t *)(_SCB_BASE + 0x8))
#define _SYSTICK_CALIB  (*(rt_uint32_t *)(_SCB_BASE + 0xC))
#define _SYSTICK_PRI    (*(rt_uint8_t  *)(0xE000ED23UL))

// Updates the variable SystemCoreClock and must be called 
// whenever the core clock is changed during program execution.
extern void SystemCoreClockUpdate(void);

// Holds the system core clock, which is the system clock 
// frequency supplied to the SysTick timer and the processor 
// core clock.
extern uint32_t SystemCoreClock;

static uint32_t _SysTick_Config(rt_uint32_t ticks)
{
    if ((ticks - 1) > 0xFFFFFF)
    {
        return 1;
    }
    
    _SYSTICK_LOAD = ticks - 1; 
    _SYSTICK_PRI = 0xFF;
    _SYSTICK_VAL  = 0;
    _SYSTICK_CTRL = 0x07;  
    
    return 0;
}
#endif
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#if defined(STM32f103VET6_small)
#define RT_HEAP_SIZE 1024*10   // ��40k��heap
#endif
#if defined(STM32f103C8T6_mostsmall)
#define RT_HEAP_SIZE 1024*3   // ��12k��heap
#endif
static uint32_t rt_heap[RT_HEAP_SIZE];	// heap default size: 6K(1536 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{	
	#if 0
	/* System Clock Update */
	SystemCoreClockUpdate();
	
	/* System Tick Configuration */
	_SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
	#endif
	
	
/*                           �û������ʼ��
*************************************************************************
*/	
	SysTick_Init(72); // ������SysTick�ľ�ȷ��ʱ����ΪӲ����ʼ��������Ҫ
	//NVIC_Configuration(); 	 // ����Ƕ�������жϿ�����NVIC��uart��ʼ���������ˣ�����Ҫ����sys.h���
	LED_Init();
	//LED_Turn0();
	KEY_Init();
	OLED_Init();			//oled��ʼ��
	OLED_Clear();         // oled����
    OLED_ShowString(0,0, "hello mcu");	 
	uart_device_init();
	USART_Cmd(camera_uart_device.uart_module, DISABLE);	 // �ر�����ͷ��uart����
	Hcsr04Init();  // ��ʼ��������
	//AT24CXX_Init();   // AT24c02��ʼ��iic
	bsp_steer_init(50);  //��ʼ�����
	pwm_set_Duty(&steer1, Steer1_S3010_mid);
	pwm_set_Duty(&steer2, Steer2_S3010_mid);
	statetable_init();  // ״̬ת�Ʊ��ʼ��
	standing_point_init();  // ����̶����ʼ��
/*                           
*************************************************************************
*/	
	SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND); // SysTick���ó�OS����
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
	rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
    
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

void SysTick_Handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
}


/**
  * @brief  ��ӳ�䴮��DEBUG_USARTx��rt_kprintf()����
  *   Note��DEBUG_USARTx����bsp_usart.h�ж���ĺ꣬Ĭ��ʹ�ô���1
  * @param  str��Ҫ��������ڵ��ַ���
  * @retval ��
  *
  * @attention
  * 
  */
void rt_hw_console_output(const char *str)
{	
	/* �����ٽ�� */
    rt_enter_critical();

	/* ֱ���ַ������� */
    while (*str!='\0')
	{
		/* ���� */
        if (*str=='\n')
		{
			USART_SendData(DEBUG_USARTx, '\r'); 
			//USART_SendData(Camera_USARTx, '\r'); 
			while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
			//while (USART_GetFlagStatus(Camera_USARTx, USART_FLAG_TXE) == RESET);
		}

		USART_SendData(DEBUG_USARTx, *str++); 	
		//USART_SendData(Camera_USARTx, *str++); 
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);	
		//while (USART_GetFlagStatus(Camera_USARTx, USART_FLAG_TXE) == RESET);
	}	

	/* �˳��ٽ�� */
    rt_exit_critical();
}
/*�߾�����ʱʵ��*/
void rt_hw_us_delay(rt_uint32_t us)
{
	rt_uint32_t delta;
	us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
	delta = SysTick->VAL;
	while(delta - SysTick->VAL< us);
}
