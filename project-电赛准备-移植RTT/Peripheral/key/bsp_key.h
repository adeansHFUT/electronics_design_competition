#ifndef _bsp_key_H
#define _bsp_key_H


#include "sys.h"
#include "rtthread.h"
 
#if defined(STM32f103VET6_small) 
#define KEY1_Pin    GPIO_Pin_0    //定义K_1管脚
#define KEY2_Pin    GPIO_Pin_1    //定义K_2管脚
#define KEY3_Pin    GPIO_Pin_2   //定义K_3管脚
#define KEY4_Pin    GPIO_Pin_3  //定义K_4管脚
#define KEY1_Port (GPIOA) //定义端口
#define KEY2_Port (GPIOA) //定义端口
#define KEY3_Port (GPIOA) //定义端口
#define KEY4_Port (GPIOA) //定义端口
//使用位操作定义
#define K_1 PAin(0)
#define K_2 PAin(1)
#define K_3 PAin(2)
#define K_4 PAin(3)
#endif
#if defined(STM32f103ZET6_alien) 
#define KEY1_Pin    GPIO_Pin_4    //定义K_1管脚
#define KEY2_Pin    GPIO_Pin_5    //定义K_2管脚
#define KEY3_Pin    GPIO_Pin_6   //定义K_3管脚
#define KEY4_Pin    GPIO_Pin_7  //定义K_4管脚
#define KEY1_Port (GPIOA) //定义端口
#define KEY2_Port (GPIOA) //定义端口
#define KEY3_Port (GPIOA) //定义端口
#define KEY4_Port (GPIOA) //定义端口
//使用位操作定义
#define K_1 PAin(4)
#define K_2 PAin(5)
#define K_3 PAin(6)
#define K_4 PAin(7)
#endif



//使用读取管脚状态库函数定义 
//#define K_UP      GPIO_ReadInputDataBit(KEY_UP_Port,KEY_UP_Pin)
//#define K_DOWN    GPIO_ReadInputDataBit(KEY_Port,KEY_DOWN_Pin)
//#define K_LEFT    GPIO_ReadInputDataBit(KEY_Port,KEY_LEFT_Pin)
//#define K_RIGHT   GPIO_ReadInputDataBit(KEY_Port,KEY_RIGHT_Pin)


//定义各个按键值  
#define KEY_1 1
#define KEY_2 2
#define KEY_3 3
#define KEY_4 4  



void KEY_Init(void);
void keyscan_callback(void *parameter);
extern rt_uint32_t key_num;
extern rt_timer_t timer_keyscan;
extern rt_mailbox_t mb_key;
#endif
