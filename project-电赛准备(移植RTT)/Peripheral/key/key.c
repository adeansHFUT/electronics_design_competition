#include "key.h"
#include "SysTick.h"

u8 key_num = 0;  
/*******************************************************************************
* �� �� ��         : KEY_Init
* ��������		   : ������ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin=KEY1_Pin|KEY2_Pin|KEY3_Pin|KEY4_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(KEY_Port,&GPIO_InitStructure);
}

/*******************************************************************************
* �� �� ��         : KEY_Scan
* ��������		   : ����ɨ����
* ��    ��         : mode=0:���ΰ��°���
					 mode=1���������°���
* ��    ��         : 0��δ�а�������
					 KEY_1��KEY_1������
					 KEY_2��KEY_2������
					 KEY_3��KEY_3������
					 KEY_4��KEY_4������
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	if(key==1&&(K_1==0||K_2==0||K_3==0||K_4==0)) //����һ����������
	{
		delay_ms(20);  //����(ԭ����10ms)
		key=0;
		if(K_1==0)
		{
			return KEY_1; 
		}
		else if(K_2==0)
		{
			return KEY_2; 
		}
		else if(K_3==0)
		{
			return KEY_3; 
		}
		else
		{
			return KEY_4;
		}
	}
	else if(K_1==1&&K_2==1&&K_3==1&&K_4==1)    //�ް�������
	{
		key=1;
	}
	if(mode==1) //������������
	{
		key=1;
	}
	return 0;
}

