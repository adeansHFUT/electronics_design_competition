#include "include.h"

int main(void)
{ 
	SysTick_Init(72);
	//LED_Init(); // PC�˿�
	NVIC_Configuration(); 	 
	OLED_Init();			//oled��ʼ��
	OLED_Clear(); 
	TIM2_Init(10,72000-1);  // 1/(72M/72000)=1ms, ��ʱ��2
	steerFrequency_Init(1);	 //�����ʼ��
	main_Display();
	return 0;
}