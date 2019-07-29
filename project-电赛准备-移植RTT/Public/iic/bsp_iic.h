#ifndef _bsp_iic_H
#define _bsp_iic_H

#include "sys.h"

/*  AT24cxx_IIC_SCL时钟端口、引脚定义 */
#define AT24cxx_IIC_SCL_PORT 			GPIOB   
#define AT24cxx_IIC_SCL_PIN 			(GPIO_Pin_6)
#define AT24cxx_IIC_SCL_PORT_RCC		RCC_APB2Periph_GPIOB

/*  AT24cxx_IIC_SDA时钟端口、引脚定义 */
#define AT24cxx_IIC_SDA_PORT 			GPIOB  
#define AT24cxx_IIC_SDA_PIN 			(GPIO_Pin_7)
#define AT24cxx_IIC_SDA_PORT_RCC		RCC_APB2Periph_GPIOB

//IO操作函数	 
#define AT24cxx_IIC_SCL    PBout(6) //SCL
#define AT24cxx_IIC_SDA    PBout(7) //SDA	 
#define AT24cxx_READ_SDA   PBin(7)  //输入SDA

typedef struct  
{
	GPIO_TypeDef* SDA_port;  
	GPIO_TypeDef* SCL_port;  //端口
	uint16_t SDA_pin;  // gpio引脚
	uint16_t SCL_pin;  // gpio引脚
	uint32_t RCC_SDAport;  //RCC时钟
	uint32_t RCC_SCLport;
	volatile unsigned long* Io_scl;  //操作的io
	volatile unsigned long* Io_sda;
	volatile unsigned long* Io_read_sda;

}My_iic_device;

//IIC所有操作函数
//void AT24cxx_IIC_Init(void);                //初始化IIC的IO口				 
//void AT24cxx_IIC_Start(void);				//发送IIC开始信号
//void AT24cxx_IIC_Stop(void);	  			//发送IIC停止信号
//void AT24cxx_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
//u8 AT24cxx_IIC_Read_Byte(u8 ack);//IIC读取一个字节
//u8 AT24cxx_IIC_Wait_Ack(void); 				//IIC等待ACK信号
//void AT24cxx_IIC_Ack(void);					//IIC发送ACK信号
//void AT24cxx_IIC_NAck(void);				//IIC不发送ACK信号

//IIC所有操作函数(改良。。。)
void IIC_Init(My_iic_device device);
void IIC_Start(My_iic_device device);				//发送IIC开始信号
void IIC_Stop(My_iic_device device);	  			//发送IIC停止信号
void IIC_Send_Byte(My_iic_device device, u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(My_iic_device device, u8 ack);//IIC读取一个字节
u8 IIC_Wait_Ack(My_iic_device device); 				//IIC等待ACK信号
void IIC_Ack(My_iic_device device);					//IIC发送ACK信号
void IIC_NAck(My_iic_device device);				//IIC不发送ACK信号
#endif
