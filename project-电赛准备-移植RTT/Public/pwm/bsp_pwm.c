#include "bsp_pwm.h"

static u16 t_arr = 0; //һ��tim��arr�Ĵ�����ֵ


/*******************************************************************************
* �� �� ��         : TIM3_CH12_PWM_Init
* ��������		   : TIM3ͨ��PWM��ʼ������(���)
* ��    ��         : per:��װ��ֵ
					 psc:��Ƶϵ��
* ��    ��         : ��
*******************************************************************************/
void TIM3_CH12_steer_PWM_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	TIM_OCInitTypeDef TIM_OCInitStructure1;//ͨ��1
	TIM_OCInitTypeDef TIM_OCInitStructure2;//ͨ��2
	GPIO_InitTypeDef GPIO_InitStructure6;//A6�ܽ�
	GPIO_InitTypeDef GPIO_InitStructure7;//A7�ܽ�

	/* ����ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ioC enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure6.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure6.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure6.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure6);
	
	GPIO_InitStructure7.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure7.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure7.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure7);
	
	
	////////////////////////////////////////////��������ܽ�����
	
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //�Զ�װ��ֵ(��������pwmƵ��)
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	
	
	TIM_OCInitStructure1.TIM_OCMode=TIM_OCMode_PWM1;  //CNT<CCRʱ������
	TIM_OCInitStructure1.TIM_OCPolarity=TIM_OCPolarity_High; //����Ϊ�ߵ�ƽ
	TIM_OCInitStructure1.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure1.TIM_Pulse = 200;
	TIM_OC1Init(TIM3,&TIM_OCInitStructure1); //����Ƚ�ͨ��1��ʼ��
	
	TIM_OCInitStructure2.TIM_OCMode=TIM_OCMode_PWM1;  
	TIM_OCInitStructure2.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure2.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure2.TIM_Pulse = 200;
	TIM_OC2Init(TIM3,&TIM_OCInitStructure2); //����Ƚ�ͨ��2��ʼ��
	

	//////////////////////�������pwm�������
	
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); //ʹ��TIMx�� CCR2 �ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); //ʹ��TIMx�� CCR2 �ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ʹ��Ԥװ�ؼĴ���

	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��
		
}
/*******************************************************************************
* �� �� ��         : PWM_Init
* ��������		   : PWM��ʼ������
* ��    ��         : per:��װ��ֵ
					 psc:��Ƶϵ��
* ��    ��         : ��
*******************************************************************************/
void PWM_Init(My_pwm_device pwm_device, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	
	/* ����ʱ�� */
	RCC_APB2PeriphClockCmd(pwm_device.pwm_port_RCC,ENABLE);//ioC enable
	RCC_APB1PeriphClockCmd(pwm_device.timx_RCC,ENABLE);

	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=pwm_device.pwm_pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_Init(pwm_device.pwm_port,&GPIO_InitStructure);
	
	
	////////////////////////////////////////////��������ܽ�����
	
	
	TIM_TimeBaseInitStructure.TIM_Period=pwm_device.pwm_arr-1;   //�Զ�װ��ֵ(��������pwmƵ��)
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(pwm_device.timx,&TIM_TimeBaseInitStructure);	
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;  //CNT<CCRʱ������
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; //����Ϊ�ߵ�ƽ
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = pwm_device.pwm_ccr;  // ��ʼccr
	switch(pwm_device.tim_channel){
		case TIM_Channel_1: TIM_OC1Init(pwm_device.timx,&TIM_OCInitStructure); //����Ƚ�ͨ��1��ʼ��
							break;
		case TIM_Channel_2: TIM_OC2Init(pwm_device.timx,&TIM_OCInitStructure); //����Ƚ�ͨ��2��ʼ��
							break;
		case TIM_Channel_3: TIM_OC3Init(pwm_device.timx,&TIM_OCInitStructure); //����Ƚ�ͨ��3��ʼ��
							break;
		case TIM_Channel_4: TIM_OC4Init(pwm_device.timx,&TIM_OCInitStructure); //����Ƚ�ͨ��4��ʼ��
							break;
	}
	
	//////////////////////�������pwm�������
	switch(pwm_device.tim_channel){
		case TIM_Channel_1: TIM_OC1PreloadConfig(pwm_device.timx,TIM_OCPreload_Enable); //ʹ��TIMx�� CCR1 �ϵĲ���ȽϼĴ���
							break;
		case TIM_Channel_2: TIM_OC2PreloadConfig(pwm_device.timx,TIM_OCPreload_Enable); //ʹ��TIMx�� CCR2 �ϵĲ���Ƚ��ؼĴ���
							break;
		case TIM_Channel_3: TIM_OC3PreloadConfig(pwm_device.timx,TIM_OCPreload_Enable); //ʹ��TIMx�� CCR3 �ϵĲ���Ƚ��ؼĴ���
							break;
		case TIM_Channel_4: TIM_OC4PreloadConfig(pwm_device.timx,TIM_OCPreload_Enable); //ʹ��TIMx�� CCR4 �ϵĲ���Ƚ��ؼĴ���
							break;
	}
	
	TIM_ARRPreloadConfig(pwm_device.timx,ENABLE);//ʹ���Զ���װ�Ĵ���

	TIM_Cmd(pwm_device.timx,ENABLE); //ʹ�ܶ�ʱ��	
}




/*                           ������
*************************************************************************
*/
/*
��������steerFrequency_Init
�������ã���ʼ�����Ƶ��
������Ƶ��.1-10����λ��kMz��
���أ���
*/
void steerFrequency_Init(u8 fre)
{
	t_arr = 1000/fre;
	TIM3_CH12_steer_PWM_Init(t_arr-1, 72-1);
}
void pwm_Frequency_Init(My_pwm_device pwm_device, u8 fre)
{
	pwm_device.pwm_arr = 1000/fre;
	pwm_device.pwm_ccr = pwm_device.pwm_arr/2;   // ��ʼccrΪarr��һ�룬%50ռ�ձ�
	PWM_Init(pwm_device, 72-1);
}
/*
��������set_steerDuty
�������ã����ö��ռ�ձ�
������0-1000����λ��ǧ��֮��
���أ���
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

