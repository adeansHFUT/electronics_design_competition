#ifndef __board_H__
#define __board_H__

#include "stm32f10x.h"
/*开发板bsp头文件*/
#include "include.h"
/**********函数声明************/
void rt_hw_board_init(void);
void SysTick_Handler(void);


#endif