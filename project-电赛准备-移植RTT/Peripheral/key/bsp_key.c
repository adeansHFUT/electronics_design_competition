#include "bsp_key.h"
#include "SysTick.h"
#include "rtthread.h"
rt_uint32_t key_num = 0;  
rt_timer_t timer_keyscan = RT_NULL; //����ɨ�趨ʱ��
rt_mailbox_t mb_key = RT_NULL;  //���ݰ���ֵ������
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

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=KEY1_Pin;
	GPIO_Init(KEY1_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=KEY2_Pin;
	GPIO_Init(KEY2_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=KEY3_Pin;
	GPIO_Init(KEY3_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=KEY4_Pin;
	GPIO_Init(KEY4_Port,&GPIO_InitStructure);
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
		rt_thread_mdelay(8);  //����(ԭ����10ms)(ע��������������ʱ������xms)
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

void keyscan_callback(void *parameter)
{
	if(key_num == 0) //Ϊ��˵�������¼��Ѿ��������Լ���ɨ�谴��
	{
		key_num = KEY_Scan(0);
		if(key_num)  // �����Ϊ0�ͷ����ʼ������������߳�
		{
			rt_err_t uwRet = rt_mb_send(mb_key, key_num);
			if(RT_EOK == uwRet)
				rt_kprintf("timer_keyscan���������ͳɹ���\n");
			
		}
			
	}
		
}
