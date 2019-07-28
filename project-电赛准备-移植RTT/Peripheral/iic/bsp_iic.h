#ifndef _bsp_iic_H
#define _bsp_iic_H

#include "sys.h"

/*  AT24cxx_IIC_SCLʱ�Ӷ˿ڡ����Ŷ��� */
#define AT24cxx_IIC_SCL_PORT 			GPIOB   
#define AT24cxx_IIC_SCL_PIN 			(GPIO_Pin_6)
#define AT24cxx_IIC_SCL_PORT_RCC		RCC_APB2Periph_GPIOB

/*  AT24cxx_IIC_SDAʱ�Ӷ˿ڡ����Ŷ��� */
#define AT24cxx_IIC_SDA_PORT 			GPIOB  
#define AT24cxx_IIC_SDA_PIN 			(GPIO_Pin_7)
#define AT24cxx_IIC_SDA_PORT_RCC		RCC_APB2Periph_GPIOB

//IO��������	 
#define AT24cxx_IIC_SCL    PBout(6) //SCL
#define AT24cxx_IIC_SDA    PBout(7) //SDA	 
#define AT24cxx_READ_SDA   PBin(7)  //����SDA

//IIC���в�������
void AT24cxx_IIC_Init(void);                //��ʼ��IIC��IO��				 
void AT24cxx_IIC_Start(void);				//����IIC��ʼ�ź�
void AT24cxx_IIC_Stop(void);	  			//����IICֹͣ�ź�
void AT24cxx_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 AT24cxx_IIC_Read_Byte(u8 ack);//IIC��ȡһ���ֽ�
u8 AT24cxx_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void AT24cxx_IIC_Ack(void);					//IIC����ACK�ź�
void AT24cxx_IIC_NAck(void);				//IIC������ACK�ź�


#endif
