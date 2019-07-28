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

//IIC所有操作函数
void AT24cxx_IIC_Init(void);                //初始化IIC的IO口				 
void AT24cxx_IIC_Start(void);				//发送IIC开始信号
void AT24cxx_IIC_Stop(void);	  			//发送IIC停止信号
void AT24cxx_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 AT24cxx_IIC_Read_Byte(u8 ack);//IIC读取一个字节
u8 AT24cxx_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void AT24cxx_IIC_Ack(void);					//IIC发送ACK信号
void AT24cxx_IIC_NAck(void);				//IIC不发送ACK信号


#endif
