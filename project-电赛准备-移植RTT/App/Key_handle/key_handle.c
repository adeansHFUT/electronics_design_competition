#include "key_handle.h"
#include "rtthread.h"
#include "bsp_key.h"
#include "display.h"

/*******************************定义现在的状态(初始为Mainmeau态)************************************************/
Statename current_state = Mainmeau; 
typedef struct {
	Statename state_name;
	Action action;
}state;
/*******************************定义状态转移表************************************************/
state state_transition[10][10]; 
/*******************************函数声明************************************************/
void state_transfer(uint8_t statenow, uint8_t key_receive);
/*******************************************************************************
* 函 数 名         : statetable_init
* 函数功能		   : 按键触发的状态转移表初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void statetable_init(void)
{
	state_transition[Mainmeau][KEY_1].state_name = Testmeau;
	state_transition[Mainmeau][KEY_1].action = Mainmeau_to_Testmeau;
	state_transition[Mainmeau][KEY_2].state_name = Task1;
	state_transition[Mainmeau][KEY_2].action = Mainmeau_to_Task1;
	state_transition[Mainmeau][KEY_3].state_name = Task2;
	state_transition[Mainmeau][KEY_3].action = Mainmeau_to_Task2;
	state_transition[Mainmeau][KEY_4].state_name = Mainmeau;
	state_transition[Mainmeau][KEY_4].action = NOaction;
	
	state_transition[Testmeau][KEY_1].state_name = Testmeau;
	state_transition[Testmeau][KEY_1].action = NOaction;
	state_transition[Testmeau][KEY_2].state_name = Testmeau;
	state_transition[Testmeau][KEY_2].action = NOaction;
	state_transition[Testmeau][KEY_3].state_name = Testmeau;
	state_transition[Testmeau][KEY_3].action = NOaction;
	state_transition[Testmeau][KEY_4].state_name = Mainmeau;
	state_transition[Testmeau][KEY_4].action = Testmeau_to_Mainmeau;
	
	state_transition[Task1][KEY_1].state_name = Task1;
	state_transition[Task1][KEY_1].action = NOaction;
	state_transition[Task1][KEY_2].state_name = Task1;
	state_transition[Task1][KEY_2].action = NOaction;
	state_transition[Task1][KEY_3].state_name = Task1;
	state_transition[Task1][KEY_3].action = NOaction;
	state_transition[Task1][KEY_4].state_name = Mainmeau;
	state_transition[Task1][KEY_4].action = Task1_to_Mainmeau;
	
	state_transition[Task2][KEY_1].state_name = Task2;
	state_transition[Task2][KEY_1].action = NOaction;
	state_transition[Task2][KEY_2].state_name = Task2;
	state_transition[Task2][KEY_2].action = NOaction;
	state_transition[Task2][KEY_3].state_name = Task2;
	state_transition[Task2][KEY_3].action = NOaction;
	state_transition[Task2][KEY_4].state_name = Mainmeau;
	state_transition[Task2][KEY_4].action = Task2_to_Mainmeau;
}
/*******************************************************************************
* 函 数 名         : keyhandle_thread_entry
* 函数功能		   : 按键处理线程入口函数
* 输    入         : 无
* 输    出         : 无
* 备    注 		   ：优先级要大于各个任务，保证实时性
*******************************************************************************/
void keyhandle_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;	
	rt_int32_t keynum;
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
		uwRet = rt_mb_recv(mb_key, &keynum, RT_WAITING_FOREVER); 	  /* 等待时间：一直 */
		if(RT_EOK == uwRet)
		{
		    rt_kprintf("收到按键:%d\n",keynum);
			state_transfer(current_state, keynum);
		}
    }
}
/*******************************************************************************
* 函 数 名         : state_transfer
* 函数功能		   : 状态转移函数
* 输    入         : 无
* 输    出         : 无
* 备    注 		   ：优先级要大于各个任务，保证实时性
*******************************************************************************/
void state_transfer(uint8_t statenow, uint8_t key_receive)
{
	if(statenow == state_transition[statenow][key_receive].state_name)
		return;
	else
	{
		switch(state_transition[statenow][key_receive].action){
			case NOaction: break;
			case Mainmeau_to_Testmeau: rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
									   break;
			case Testmeau_to_Mainmeau: rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
									   break;
			case Mainmeau_to_Task1: rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
									//这里放对其他任务的操作
									break;
			case Task1_to_Mainmeau: rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
									//这里放对其他任务的操作
									break;
			case Mainmeau_to_Task2: rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
									//这里放对其他任务的操作
									break;
			case Task2_to_Mainmeau: rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
									//这里放对其他任务的操作
									break;
			default: break;
		}
		rt_enter_critical(); // 直接关调度器，好狠
		current_state = state_transition[statenow][key_receive].state_name; //更新状态
		rt_exit_critical();
	}
		
}