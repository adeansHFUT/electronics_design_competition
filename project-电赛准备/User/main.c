#include "include.h"

int main(void)
{ 
	SysTick_Init(72);
	//LED_Init(); // PC端口
	NVIC_Configuration(); 	 
	OLED_Init();			//oled初始化
	OLED_Clear(); 
	TIM2_Init(10,72000-1);  // 1/(72M/72000)=1ms, 定时器2
	steerFrequency_Init(1);	 //舵机初始化
	main_Display();
	return 0;
}