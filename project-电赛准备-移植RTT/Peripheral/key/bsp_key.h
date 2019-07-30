#ifndef _bsp_key_H
#define _bsp_key_H


#include "sys.h"
#include "rtthread.h"
 
#if defined(STM32f103VET6_small) 
#define KEY1_Pin    GPIO_Pin_0    //����K_1�ܽ�
#define KEY2_Pin    GPIO_Pin_1    //����K_2�ܽ�
#define KEY3_Pin    GPIO_Pin_2   //����K_3�ܽ�
#define KEY4_Pin    GPIO_Pin_3  //����K_4�ܽ�
#define KEY1_Port (GPIOA) //����˿�
#define KEY2_Port (GPIOA) //����˿�
#define KEY3_Port (GPIOA) //����˿�
#define KEY4_Port (GPIOA) //����˿�
//ʹ��λ��������
#define K_1 PAin(0)
#define K_2 PAin(1)
#define K_3 PAin(2)
#define K_4 PAin(3)
#endif
#if defined(STM32f103ZET6_alien) 
#define KEY1_Pin    GPIO_Pin_4    //����K_1�ܽ�
#define KEY2_Pin    GPIO_Pin_5    //����K_2�ܽ�
#define KEY3_Pin    GPIO_Pin_6   //����K_3�ܽ�
#define KEY4_Pin    GPIO_Pin_7  //����K_4�ܽ�
#define KEY1_Port (GPIOA) //����˿�
#define KEY2_Port (GPIOA) //����˿�
#define KEY3_Port (GPIOA) //����˿�
#define KEY4_Port (GPIOA) //����˿�
//ʹ��λ��������
#define K_1 PAin(4)
#define K_2 PAin(5)
#define K_3 PAin(6)
#define K_4 PAin(7)
#endif



//ʹ�ö�ȡ�ܽ�״̬�⺯������ 
//#define K_UP      GPIO_ReadInputDataBit(KEY_UP_Port,KEY_UP_Pin)
//#define K_DOWN    GPIO_ReadInputDataBit(KEY_Port,KEY_DOWN_Pin)
//#define K_LEFT    GPIO_ReadInputDataBit(KEY_Port,KEY_LEFT_Pin)
//#define K_RIGHT   GPIO_ReadInputDataBit(KEY_Port,KEY_RIGHT_Pin)


//�����������ֵ  
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
