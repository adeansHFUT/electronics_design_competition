#include "bsp_iic.h"
#include "SysTick.h"
#include "rtthread.h"
#include "board.h"

/*******************************************************************************
* �� �� ��         : IIC_Init
* ��������		   : IIC��ʼ��
* ��    ��         : ��
* ��    ��         : ��
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
* �� �� ��         : SDA_OUT
* ��������		   : SDA�������	   
* ��    ��         : ��
* ��    ��         : ��
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
* �� �� ��         : SDA_IN
* ��������		   : SDA��������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=AT24cxx_IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(AT24cxx_IIC_SDA_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* �� �� ��         : 24cxx_IIC_Start
* ��������		   : ����IIC��ʼ�ź�   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void AT24cxx_IIC_Start(void)
{
	SDA_OUT();     //sda�����
	AT24cxx_IIC_SDA=1;	  	  
	AT24cxx_IIC_SCL=1;
	rt_hw_us_delay(5);
 	AT24cxx_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	rt_hw_us_delay(6);
	AT24cxx_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	

/*******************************************************************************
* �� �� ��         : AT24cxx_IIC_Stop
* ��������		   : ����IICֹͣ�ź�   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void AT24cxx_IIC_Stop(void)
{
	SDA_OUT();//sda�����
	AT24cxx_IIC_SCL=0;
	AT24cxx_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	AT24cxx_IIC_SCL=1; 
	rt_hw_us_delay(6); 
	AT24cxx_IIC_SDA=1;//����I2C���߽����ź�
	rt_hw_us_delay(6);							   	
}

/*******************************************************************************
* �� �� ��         : AT24cxx_IIC_Wait_Ack
* ��������		   : �ȴ�Ӧ���źŵ���   
* ��    ��         : ��
* ��    ��         : 1������Ӧ��ʧ��
        			 0������Ӧ��ɹ�
*******************************************************************************/
u8 AT24cxx_IIC_Wait_Ack(void)
{
	u8 tempTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	AT24cxx_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 

/*******************************************************************************
* �� �� ��         : AT24cxx_IIC_Ack
* ��������		   : ����ACKӦ��  
* ��    ��         : ��
* ��    ��         : ��
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
* �� �� ��         : AT24cxx_IIC_NAck
* ��������		   : ����NACK��Ӧ��  
* ��    ��         : ��
* ��    ��         : ��
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
* �� �� ��         : AT24cxx_IIC_Send_Byte
* ��������		   : IIC����һ���ֽ� 
* ��    ��         : txd������һ���ֽ�
* ��    ��         : ��
*******************************************************************************/		  
void AT24cxx_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    AT24cxx_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>0) //0x80  1000 0000
			AT24cxx_IIC_SDA=1;
		else
			AT24cxx_IIC_SDA=0;
        txd<<=1; 	  
		rt_hw_us_delay(2);   //��TEA5767��������ʱ���Ǳ����
		AT24cxx_IIC_SCL=1;
		rt_hw_us_delay(2); 
		AT24cxx_IIC_SCL=0;	
		rt_hw_us_delay(2);
    }	 
} 

/*******************************************************************************
* �� �� ��         : AT24cxx_IIC_Read_Byte
* ��������		   : IIC��һ���ֽ� 
* ��    ��         : ack=1ʱ������ACK��ack=0������nACK 
* ��    ��         : Ӧ����Ӧ��
*******************************************************************************/  
u8 AT24cxx_IIC_Read_Byte(u8 ack)
{
	u8 i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        AT24cxx_IIC_NAck();//����nACK
    else
        AT24cxx_IIC_Ack(); //����ACK   
    return receive;
}






