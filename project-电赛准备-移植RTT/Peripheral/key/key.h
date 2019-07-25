#ifndef _key_H
#define _key_H


#include "sys.h"
 
#define KEY1_Pin    GPIO_Pin_0    //����K_1�ܽ�
#define KEY2_Pin    GPIO_Pin_1    //����K_2�ܽ�
#define KEY3_Pin    GPIO_Pin_2   //����K_3�ܽ�
#define KEY4_Pin    GPIO_Pin_3  //����K_4�ܽ�

#define KEY_Port (GPIOA) //����˿�



//ʹ��λ��������
#define K_1 PAin(0)
#define K_2 PAin(1)
#define K_3 PAin(2)
#define K_4 PAin(3)

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
u8 KEY_Scan(u8 mode);
#endif
