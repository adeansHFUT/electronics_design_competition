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
#define RT_HEAP_SIZE 1024*10   // 分40k给heap
#endif
#if defined(STM32f103C8T6_mostsmall)
#define RT_HEAP_SIZE 1024*3   // 分12k给heap
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
	
	
/*                           用户外设初始化
*************************************************************************
*/	
	SysTick_Init(72); // 先用下SysTick的精确延时，因为硬件初始化可能需要
	//NVIC_Configuration(); 	 // 配置嵌套向量中断控制器NVIC，uart初始化里面有了，不需要再用sys.h里的
	LED_Init();
	//LED_Turn0();
	KEY_Init();
	OLED_Init();			//oled初始化
	OLED_Clear();         // oled清屏
    OLED_ShowString(0,0, "hello mcu");	 
	uart_device_init();
	USART_Cmd(camera_uart_device.uart_module, DISABLE);	 // 关闭摄像头的uart接收
	Hcsr04Init();  // 初始化超声波
	//AT24CXX_Init();   // AT24c02初始化iic
	bsp_steer_init(50);  //初始化舵机
	pwm_set_Duty(&steer1, Steer1_S3010_mid);
	pwm_set_Duty(&steer2, Steer2_S3010_mid);
	statetable_init();  // 状态转移表初始化
	standing_point_init();  // 板球固定点初始化
/*                           
*************************************************************************
*/	
	SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND); // SysTick配置成OS心跳
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
  * @brief  重映射串口DEBUG_USARTx到rt_kprintf()函数
  *   Note：DEBUG_USARTx是在bsp_usart.h中定义的宏，默认使用串口1
  * @param  str：要输出到串口的字符串
  * @retval 无
  *
  * @attention
  * 
  */
void rt_hw_console_output(const char *str)
{	
	/* 进入临界段 */
    rt_enter_critical();

	/* 直到字符串结束 */
    while (*str!='\0')
	{
		/* 换行 */
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

	/* 退出临界段 */
    rt_exit_critical();
}
/*高精度延时实现*/
void rt_hw_us_delay(rt_uint32_t us)
{
	rt_uint32_t delta;
	us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
	delta = SysTick->VAL;
	while(delta - SysTick->VAL< us);
}
