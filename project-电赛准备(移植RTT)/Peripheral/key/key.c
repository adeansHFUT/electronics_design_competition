#include "key.h"
#include "SysTick.h"

u8 key_num = 0;  
/*******************************************************************************
* 函 数 名         : KEY_Init
* 函数功能		   : 按键初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Pin=KEY1_Pin|KEY2_Pin|KEY3_Pin|KEY4_Pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(KEY_Port,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : KEY_Scan
* 函数功能		   : 按键扫描检测
* 输    入         : mode=0:单次按下按键
					 mode=1：连续按下按键
* 输    出         : 0：未有按键按下
					 KEY_1：KEY_1键按下
					 KEY_2：KEY_2键按下
					 KEY_3：KEY_3键按下
					 KEY_4：KEY_4键按下
*******************************************************************************/
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	if(key==1&&(K_1==0||K_2==0||K_3==0||K_4==0)) //任意一个按键按下
	{
		delay_ms(20);  //消抖(原来是10ms)
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
	else if(K_1==1&&K_2==1&&K_3==1&&K_4==1)    //无按键按下
	{
		key=1;
	}
	if(mode==1) //连续按键按下
	{
		key=1;
	}
	return 0;
}

