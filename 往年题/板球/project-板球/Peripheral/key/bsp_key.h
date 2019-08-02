#ifndef _bsp_key_H
#define _bsp_key_H


#include "sys.h"
#include "rtthread.h"
#include "PORT_cfg.h"




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
#define KEY_5 5
#define KEY_6 6
#define KEY_7 7
#define KEY_8 8  



void KEY_Init(void);
void keyscan_callback(void *parameter);
extern rt_uint32_t key_num;
extern rt_timer_t timer_keyscan;
extern rt_mailbox_t mb_key;
#endif
