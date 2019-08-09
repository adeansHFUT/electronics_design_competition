#include "bsp_led.h"
#include "PORT_cfg.h"

/*******************************************************************************
* 函 数 名         : LED_Init
* 函数功能		   : LED初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(LED_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=LED_PIN;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(LED_PORT,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_SetBits(LED_PORT,LED_PIN);   //将LED端口拉高，熄灭所有LED
}

void LED_Turn0(void)
{
	GPIO_ResetBits(LED_PORT,LED_PIN);   //将LED端口0
}

void LED_Turn1(void)
{
	GPIO_SetBits(LED_PORT,LED_PIN);   //将LED端口1
}
/*******************************************************************************
* 函 数 名         : Fire_Init
* 函数功能		   : 开火口初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Fire_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(Fire_port_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=Fire_pin;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(Fire_port,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_ResetBits(Fire_port,Fire_pin);   //将fire端口拉低
}
/*******************************************************************************
* 函 数 名         : Fire_Init
* 函数功能		   : 开火口初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Charge_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义结构体变量
	
	RCC_APB2PeriphClockCmd(Charge_port_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=Charge_pin;  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  //设置传输速率
	GPIO_Init(Charge_port,&GPIO_InitStructure); 	   /* 初始化GPIO */
	
	GPIO_SetBits(Charge_port,Charge_pin);   //将Charge端口拉高充电
}