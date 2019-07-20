#include "time.h"
#include "oled.h"
#include "key.h"
extern show_node pagetable[16];
extern u8 key_num;  
static u8 x_ms = 0;
/*******************************************************************************
* 函 数 名         : TIM2_Init
* 函数功能		   : TIM4初始化函数
* 输    入         : per:重装载值
					 psc:分频系数
* 输    出         : 无
*******************************************************************************/
void TIM2_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//使能TIM2时钟
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //开启定时器中断
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIM2,ENABLE); //使能定时器	
}

/*******************************************************************************
* 函 数 名         : TIM2_IRQHandler
* 函数功能		   : TIM2中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update))
	{		
		x_ms++;  // 目前是10ms加1
		if(x_ms > 100)
			x_ms = 0;
		
		if(x_ms%40 == 1 && autoRefresh )
		{
			OLED_Clear();
			showpage(pagetable, 1, 12);
		}
		if(x_ms%20 == 2 && key_num == 0)
			key_num = KEY_Scan(0);
		
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);	
}


