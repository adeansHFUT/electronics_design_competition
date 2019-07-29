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

typedef struct  
{
	GPIO_TypeDef* SDA_port;  
	GPIO_TypeDef* SCL_port;  //�˿�
	uint16_t SDA_pin;  // gpio����
	uint16_t SCL_pin;  // gpio����
	uint32_t RCC_SDAport;  //RCCʱ��
	uint32_t RCC_SCLport;
	volatile unsigned long* Io_scl;  //������io
	volatile unsigned long* Io_sda;
	volatile unsigned long* Io_read_sda;

}My_iic_device;

//IIC���в�������
//void AT24cxx_IIC_Init(void);                //��ʼ��IIC��IO��				 
//void AT24cxx_IIC_Start(void);				//����IIC��ʼ�ź�
//void AT24cxx_IIC_Stop(void);	  			//����IICֹͣ�ź�
//void AT24cxx_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
//u8 AT24cxx_IIC_Read_Byte(u8 ack);//IIC��ȡһ���ֽ�
//u8 AT24cxx_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
//void AT24cxx_IIC_Ack(void);					//IIC����ACK�ź�
//void AT24cxx_IIC_NAck(void);				//IIC������ACK�ź�

//IIC���в�������(����������)
void IIC_Init(My_iic_device device);
void IIC_Start(My_iic_device device);				//����IIC��ʼ�ź�
void IIC_Stop(My_iic_device device);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(My_iic_device device, u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(My_iic_device device, u8 ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(My_iic_device device); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(My_iic_device device);					//IIC����ACK�ź�
void IIC_NAck(My_iic_device device);				//IIC������ACK�ź�
#endif
