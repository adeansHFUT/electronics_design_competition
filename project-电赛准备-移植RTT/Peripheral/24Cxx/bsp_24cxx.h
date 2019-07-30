#ifndef _bsp_24cxx_H
#define _bsp_24cxx_H

#include "sys.h"
#include "bsp_iic.h"


#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  

//开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE AT24C02
/****************AT24C02端口定义*************************/
#if defined(STM32f103VET6_small)
/*  AT24cxx_IIC_SCL时钟端口、引脚定义 */
#define AT24cxx_IIC_SCL_PORT 			GPIOE   
#define AT24cxx_IIC_SCL_PIN 			(GPIO_Pin_11)
#define AT24cxx_IIC_SCL_PORT_RCC		RCC_APB2Periph_GPIOE

/*  AT24cxx_IIC_SDA时钟端口、引脚定义 */
#define AT24cxx_IIC_SDA_PORT 			GPIOE  
#define AT24cxx_IIC_SDA_PIN 			(GPIO_Pin_12)
#define AT24cxx_IIC_SDA_PORT_RCC		RCC_APB2Periph_GPIOE

//IO操作函数	 
#define AT24cxx_IIC_SCL    PEout(11) //SCL
#define AT24cxx_IIC_SDA    PEout(12) //SDA	 
#define AT24cxx_READ_SDA   PEin(12)  //输入SDA

#endif
#if defined(STM32f103ZET6_alien)
/*  AT24cxx_IIC_SCL时钟端口、引脚定义 */
#define AT24cxx_IIC_SCL_PORT 			GPIOE   
#define AT24cxx_IIC_SCL_PIN 			(GPIO_Pin_11)
#define AT24cxx_IIC_SCL_PORT_RCC		RCC_APB2Periph_GPIOE

/*  AT24cxx_IIC_SDA时钟端口、引脚定义 */
#define AT24cxx_IIC_SDA_PORT 			GPIOE  
#define AT24cxx_IIC_SDA_PIN 			(GPIO_Pin_12)
#define AT24cxx_IIC_SDA_PORT_RCC		RCC_APB2Periph_GPIOE

//IO操作函数	 
#define AT24cxx_IIC_SCL    PEout(11) //SCL
#define AT24cxx_IIC_SDA    PEout(12) //SDA	 
#define AT24cxx_READ_SDA   PEin(12)  //输入SDA

#endif
u8 AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void);  //检查器件
void AT24CXX_Init(void); //初始化IIC


#endif
