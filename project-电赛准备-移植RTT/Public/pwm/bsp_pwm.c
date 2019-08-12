#include "bsp_pwm.h"

static u16 t_arr = 0; //一个tim的arr寄存器的值


/*******************************************************************************
* 函 数 名         : TIM3_CH12_PWM_Init
* 函数功能		   : TIM3通道PWM初始化函数(舵机)
* 输    入         : per:重装载值
					 psc:分频系数
* 输    出         : 无
*******************************************************************************/
void TIM3_CH12_steer_PWM_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	TIM_OCInitTypeDef TIM_OCInitStructure1;//通道1
	TIM_OCInitTypeDef TIM_OCInitStructure2;//通道2
	GPIO_InitTypeDef GPIO_InitStructure6;//A6管脚
	GPIO_InitTypeDef GPIO_InitStructure7;//A7管脚

	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ioC enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure6.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure6.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure6.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure6);
	
	GPIO_InitStructure7.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure7.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure7.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure7);
	
	
	////////////////////////////////////////////以上完出管脚配置
	
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //自动装载值(用于设置pwm频率)
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	
	
	TIM_OCInitStructure1.TIM_OCMode=TIM_OCMode_PWM1;  //CNT<CCR时起作用
	TIM_OCInitStructure1.TIM_OCPolarity=TIM_OCPolarity_High; //作用为高电平
	TIM_OCInitStructure1.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure1.TIM_Pulse = 200;
	TIM_OC1Init(TIM3,&TIM_OCInitStructure1); //输出比较通道1初始化
	
	TIM_OCInitStructure2.TIM_OCMode=TIM_OCMode_PWM1;  
	TIM_OCInitStructure2.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure2.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure2.TIM_Pulse = 200;
	TIM_OC2Init(TIM3,&TIM_OCInitStructure2); //输出比较通道2初始化
	

	//////////////////////以上完出pwm输出配置
	
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能TIMx在 CCR2 上的预装载寄存器
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能TIMx在 CCR2 上的预装载寄存器
	TIM_ARRPreloadConfig(TIM3,ENABLE);//使能预装载寄存器

	TIM_Cmd(TIM3,ENABLE); //使能定时器
		
}
/*******************************************************************************
* 函 数 名         : PWM_Init
* 函数功能		   : PWM初始化函数
* 输    入         : per:重装载值
					 psc:分频系数
* 输    出         : 无
*******************************************************************************/
void PWM_Init(My_pwm_device pwm_device, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	
	/* 开启时钟 */
	RCC_APB2PeriphClockCmd(pwm_device.pwm_port_RCC,ENABLE);//ioC enable
	RCC_APB1PeriphClockCmd(pwm_device.timx_RCC,ENABLE);

	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=pwm_device.pwm_pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_Init(pwm_device.pwm_port,&GPIO_InitStructure);
	
	
	////////////////////////////////////////////以上完出管脚配置
	
	
	TIM_TimeBaseInitStructure.TIM_Period=pwm_device.pwm_arr-1;   //自动装载值(用于设置pwm频率)
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(pwm_device.timx,&TIM_TimeBaseInitStructure);	
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //CNT<CCR时起作用
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; //作用为高电平
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = pwm_device.pwm_ccr;  // 初始ccr
	switch(pwm_device.tim_channel){
		case TIM_Channel_1: TIM_OC1Init(pwm_device.timx,&TIM_OCInitStructure); //输出比较通道1初始化
							break;
		case TIM_Channel_2: TIM_OC2Init(pwm_device.timx,&TIM_OCInitStructure); //输出比较通道2初始化
							break;
		case TIM_Channel_3: TIM_OC3Init(pwm_device.timx,&TIM_OCInitStructure); //输出比较通道3初始化
							break;
		case TIM_Channel_4: TIM_OC4Init(pwm_device.timx,&TIM_OCInitStructure); //输出比较通道4初始化
							break;
	}
	
	//////////////////////以上完出pwm输出配置
	switch(pwm_device.tim_channel){
		case TIM_Channel_1: TIM_OC1PreloadConfig(pwm_device.timx,TIM_OCPreload_Enable); //使能TIMx在 CCR1 上的捕获比较寄存器
							break;
		case TIM_Channel_2: TIM_OC2PreloadConfig(pwm_device.timx,TIM_OCPreload_Enable); //使能TIMx在 CCR2 上的捕获比较载寄存器
							break;
		case TIM_Channel_3: TIM_OC3PreloadConfig(pwm_device.timx,TIM_OCPreload_Enable); //使能TIMx在 CCR3 上的捕获比较载寄存器
							break;
		case TIM_Channel_4: TIM_OC4PreloadConfig(pwm_device.timx,TIM_OCPreload_Enable); //使能TIMx在 CCR4 上的捕获比较载寄存器
							break;
	}
	
	TIM_ARRPreloadConfig(pwm_device.timx,ENABLE);//使能自动重装寄存器

	TIM_Cmd(pwm_device.timx,ENABLE); //使能定时器	
}




/*                           舵机相关
*************************************************************************
*/
/*
函数名：steerFrequency_Init
函数作用：初始化舵机频率
参数：频率.1-10（单位是kMz）
返回：无
*/
void steerFrequency_Init(u8 fre)
{
	t_arr = 1000/fre;
	TIM3_CH12_steer_PWM_Init(t_arr-1, 72-1);
}
void pwm_Frequency_Init(My_pwm_device pwm_device, u8 fre)
{
	pwm_device.pwm_arr = 1000/fre;
	pwm_device.pwm_ccr = pwm_device.pwm_arr/2;   // 初始ccr为arr的一半，%50占空比
	PWM_Init(pwm_device, 72-1);
}
/*
函数名：set_steerDuty
函数作用：设置舵机占空比
参数：0-1000（单位是千分之）
返回：无
*/
void set_steerDuty(u8 index, uint16_t duty)
{
	uint16_t ccr;
	ccr = (duty*t_arr)/1000;
	switch(index){
		case 1: TIM_SetCompare1(TIM3, ccr);
				break;
		case 2: TIM_SetCompare2(TIM3, ccr);
				break;
		default: break;
	}
}
void pwm_set_Duty(My_pwm_device pwm_device, uint16_t duty)
{
	pwm_device.pwm_ccr = (duty*pwm_device.pwm_arr)/1000;
	switch(pwm_device.tim_channel){
		case TIM_Channel_1: TIM_SetCompare1(pwm_device.timx, pwm_device.pwm_ccr);
				break;
		case TIM_Channel_2: TIM_SetCompare2(pwm_device.timx, pwm_device.pwm_ccr);
				break;
		case TIM_Channel_3: TIM_SetCompare3(pwm_device.timx, pwm_device.pwm_ccr);
				break;
		case TIM_Channel_4: TIM_SetCompare4(pwm_device.timx, pwm_device.pwm_ccr);
				break;
		default: break;
	}
}
/*                          
*************************************************************************
*/

