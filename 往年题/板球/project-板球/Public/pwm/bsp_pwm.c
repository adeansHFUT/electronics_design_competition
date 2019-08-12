#include "bsp_pwm.h"

static u16 t_arr = 0; //һ��tim��arr�Ĵ�����ֵ


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
��������steer_pwm_Frequency_Init
�������ã���ʼ�����Ƶ��
������Ƶ��.1-1000����λ��Mz��һ��50
���أ���
*/

void steer_pwm_Frequency_Init(My_pwm_device_t pwm_device_t, uint16_t fre)
{
	pwm_device_t->pwm_arr = 60000/fre;                // arrԽ��pwm���Ⱦ�Խ�ߣ����Ҳ��ǧ��֮һ������ĺ��������ˣ�
	pwm_device_t->pwm_ccr = pwm_device_t->pwm_arr*0;  // ��֤60000*1200 = 70M�Ϳ�����
	PWM_Init(*pwm_device_t, 1200-1);
}
/*
��������steer_pwm_Frequency_Init
�������ã���ʼ�����Ƶ��
������Ƶ��.1-1000����λ�ǰ�Mz��һ��10
���أ���
*/

void motor_pwm_Frequency_Init(My_pwm_device_t pwm_device_t, uint16_t fre)
{
	pwm_device_t->pwm_arr = 10000/fre;                // arrԽ��pwm���Ⱦ�Խ�ߣ����Ҳ��ǧ��֮һ������ĺ��������ˣ�
	pwm_device_t->pwm_ccr = pwm_device_t->pwm_arr*0;  // ��֤60000*1200 = 70M�Ϳ�����
	PWM_Init(*pwm_device_t, 72-1);
}
/*
��������set_steerDuty
�������ã����ö��ռ�ձ�
������0-1000����λ��ǧ��֮��
���أ���
*/

void pwm_set_Duty(My_pwm_device_t pwm_device_t, uint16_t duty)
{
	pwm_device_t->pwm_ccr = duty*0.001*pwm_device_t->pwm_arr;
	switch(pwm_device_t->tim_channel){
		case TIM_Channel_1: TIM_SetCompare1(pwm_device_t->timx, pwm_device_t->pwm_ccr);
				break;
		case TIM_Channel_2: TIM_SetCompare2(pwm_device_t->timx, pwm_device_t->pwm_ccr);
				break;
		case TIM_Channel_3: TIM_SetCompare3(pwm_device_t->timx, pwm_device_t->pwm_ccr);
				break;
		case TIM_Channel_4: TIM_SetCompare4(pwm_device_t->timx, pwm_device_t->pwm_ccr);
				break;
		default: break;
	}
}
/*                          
*************************************************************************
*/

