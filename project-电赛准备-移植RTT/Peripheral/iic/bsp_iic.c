#include "bsp_iic.h"
#include "SysTick.h"
#include "rtthread.h"
#include "board.h"

/*******************************************************************************
* 函 数 名         : IIC_Init
* 函数功能		   : IIC初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void AT24cxx_IIC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(AT24cxx_IIC_SCL_PORT_RCC|AT24cxx_IIC_SDA_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=AT24cxx_IIC_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(AT24cxx_IIC_SCL_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AT24cxx_IIC_SDA_PIN;
	GPIO_Init(AT24cxx_IIC_SDA_PORT,&GPIO_InitStructure);
	
	AT24cxx_IIC_SCL=1;
	AT24cxx_IIC_SDA=1;	
}

/*******************************************************************************
* 函 数 名         : SDA_OUT
* 函数功能		   : SDA输出配置	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=AT24cxx_IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(AT24cxx_IIC_SDA_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : SDA_IN
* 函数功能		   : SDA输入配置	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=AT24cxx_IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(AT24cxx_IIC_SDA_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : 24cxx_IIC_Start
* 函数功能		   : 产生IIC起始信号   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void AT24cxx_IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	AT24cxx_IIC_SDA=1;	  	  
	AT24cxx_IIC_SCL=1;
	rt_hw_us_delay(5);
 	AT24cxx_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	rt_hw_us_delay(6);
	AT24cxx_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	

/*******************************************************************************
* 函 数 名         : AT24cxx_IIC_Stop
* 函数功能		   : 产生IIC停止信号   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void AT24cxx_IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	AT24cxx_IIC_SCL=0;
	AT24cxx_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	AT24cxx_IIC_SCL=1; 
	rt_hw_us_delay(6); 
	AT24cxx_IIC_SDA=1;//发送I2C总线结束信号
	rt_hw_us_delay(6);							   	
}

/*******************************************************************************
* 函 数 名         : AT24cxx_IIC_Wait_Ack
* 函数功能		   : 等待应答信号到来   
* 输    入         : 无
* 输    出         : 1，接收应答失败
        			 0，接收应答成功
*******************************************************************************/
u8 AT24cxx_IIC_Wait_Ack(void)
{
	u8 tempTime=0;
	SDA_IN();      //SDA设置为输入  
	AT24cxx_IIC_SDA=1;
	rt_hw_us_delay(1);	   
	AT24cxx_IIC_SCL=1;
	rt_hw_us_delay(1);	 
	while(AT24cxx_READ_SDA)
	{
		tempTime++;
		if(tempTime>250)
		{
			AT24cxx_IIC_Stop();
			return 1;
		}
	}
	AT24cxx_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 

/*******************************************************************************
* 函 数 名         : AT24cxx_IIC_Ack
* 函数功能		   : 产生ACK应答  
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void AT24cxx_IIC_Ack(void)
{
	AT24cxx_IIC_SCL=0;
	SDA_OUT();
	AT24cxx_IIC_SDA=0;
	rt_hw_us_delay(2);
	AT24cxx_IIC_SCL=1;
	rt_hw_us_delay(5);
	AT24cxx_IIC_SCL=0;
}

/*******************************************************************************
* 函 数 名         : AT24cxx_IIC_NAck
* 函数功能		   : 产生NACK非应答  
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/		    
void AT24cxx_IIC_NAck(void)
{
	AT24cxx_IIC_SCL=0;
	SDA_OUT();
	AT24cxx_IIC_SDA=1;
	rt_hw_us_delay(2);
	AT24cxx_IIC_SCL=1;
	rt_hw_us_delay(5);
	AT24cxx_IIC_SCL=0;
}	

/*******************************************************************************
* 函 数 名         : AT24cxx_IIC_Send_Byte
* 函数功能		   : IIC发送一个字节 
* 输    入         : txd：发送一个字节
* 输    出         : 无
*******************************************************************************/		  
void AT24cxx_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    AT24cxx_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>0) //0x80  1000 0000
			AT24cxx_IIC_SDA=1;
		else
			AT24cxx_IIC_SDA=0;
        txd<<=1; 	  
		rt_hw_us_delay(2);   //对TEA5767这三个延时都是必须的
		AT24cxx_IIC_SCL=1;
		rt_hw_us_delay(2); 
		AT24cxx_IIC_SCL=0;	
		rt_hw_us_delay(2);
    }	 
} 

/*******************************************************************************
* 函 数 名         : AT24cxx_IIC_Read_Byte
* 函数功能		   : IIC读一个字节 
* 输    入         : ack=1时，发送ACK，ack=0，发送nACK 
* 输    出         : 应答或非应答
*******************************************************************************/  
u8 AT24cxx_IIC_Read_Byte(u8 ack)
{
	u8 i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        AT24cxx_IIC_SCL=0; 
        rt_hw_us_delay(2);
		AT24cxx_IIC_SCL=1;
        receive<<=1;
        if(AT24cxx_READ_SDA)receive++;   
		rt_hw_us_delay(1); 
    }					 
    if (!ack)
        AT24cxx_IIC_NAck();//发送nACK
    else
        AT24cxx_IIC_Ack(); //发送ACK   
    return receive;
}






