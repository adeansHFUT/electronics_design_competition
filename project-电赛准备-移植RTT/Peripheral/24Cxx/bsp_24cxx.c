#include "bsp_24cxx.h"
#include "SysTick.h"
#include "rtthread.h"

/*******************************************************************************
* �� �� ��         : AT24CXX_Init
* ��������		   : AT24CXX��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void AT24CXX_Init(void)
{
	AT24cxx_IIC_Init();//IIC��ʼ��
}

/*******************************************************************************
* �� �� ��         : AT24CXX_ReadOneByte
* ��������		   : ��AT24CXXָ����ַ����һ������
* ��    ��         : ReadAddr:��ʼ�����ĵ�ַ 
* ��    ��         : ����������
*******************************************************************************/
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    AT24cxx_IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		AT24cxx_IIC_Send_Byte(0XA0);	   //����д����
		AT24cxx_IIC_Wait_Ack();
		AT24cxx_IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
	}
	else 
	{
		AT24cxx_IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д����
	} 	   
	AT24cxx_IIC_Wait_Ack(); 
    AT24cxx_IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	AT24cxx_IIC_Wait_Ack();	    
	AT24cxx_IIC_Start();  	 	   
	AT24cxx_IIC_Send_Byte(0XA1);           //�������ģʽ			   
	AT24cxx_IIC_Wait_Ack();	 
    temp=AT24cxx_IIC_Read_Byte(0);		   
    AT24cxx_IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}

/*******************************************************************************
* �� �� ��         : AT24CXX_WriteOneByte
* ��������		   : ��AT24CXXָ����ַд��һ������
* ��    ��         : WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ 
					 DataToWrite:Ҫд�������
* ��    ��         : ��
*******************************************************************************/
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    AT24cxx_IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		AT24cxx_IIC_Send_Byte(0XA0);	    //����д����
		AT24cxx_IIC_Wait_Ack();
		AT24cxx_IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}
	else 
	{
		AT24cxx_IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д����
	} 	 
	AT24cxx_IIC_Wait_Ack();	   
    AT24cxx_IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	AT24cxx_IIC_Wait_Ack(); 	 										  		   
	AT24cxx_IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	AT24cxx_IIC_Wait_Ack();  		    	   
    AT24cxx_IIC_Stop();//����һ��ֹͣ���� 
	rt_thread_mdelay(10);	 
}

/*******************************************************************************
* �� �� ��         : AT24CXX_WriteLenByte
* ��������		   : ��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
					 ����д��16bit����32bit������
* ��    ��         : WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ 
					 DataToWrite:Ҫд�������
					 Len        :Ҫд�����ݵĳ���2,4
* ��    ��         : ��
*******************************************************************************/
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

/*******************************************************************************
* �� �� ��         : AT24CXX_ReadLenByte
* ��������		   : ��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
					 ���ڶ���16bit����32bit������
* ��    ��         : ReadAddr   :��ʼ�����ĵ�ַ 
					 Len        :Ҫ�������ݵĳ���2,4
* ��    ��         : ��ȡ������
*******************************************************************************/
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}

/*******************************************************************************
* �� �� ��         : AT24CXX_Check
* ��������		   : ���AT24CXX�Ƿ�����
* ��    ��         : ��
* ��    ��         : 1:���ʧ�ܣ�0:���ɹ�
* ��    ע         �������߳����ã�����board.c��
*******************************************************************************/
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0x36)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X36);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X36)return 0;
	}
	return 1;											  
}

/*******************************************************************************
* �� �� ��         : AT24CXX_Read
* ��������		   : ��AT24CXX�����ָ����ַ��ʼ����ָ������������
* ��    ��         : ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
					 pBuffer  :���������׵�ַ
					 NumToRead:Ҫ�������ݵĸ���
* ��    ��         : ��
*******************************************************************************/
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
} 

/*******************************************************************************
* �� �� ��         : AT24CXX_Write
* ��������		   : ��AT24CXX�����ָ����ַ��ʼд��ָ������������
* ��    ��         : WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
					 pBuffer  :���������׵�ַ
					 NumToRead:Ҫ�������ݵĸ���
* ��    ��         : ��
*******************************************************************************/
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}




