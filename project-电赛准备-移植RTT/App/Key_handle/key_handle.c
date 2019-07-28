#include "key_handle.h"
#include "rtthread.h"
#include "board.h"
#include "include.h"

/* 定义按键处理线程控制块 */
rt_thread_t keyhandle_thread = RT_NULL;

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
	state_transition[Mainmeau][KEY_2].state_name = Task_randw;
	state_transition[Mainmeau][KEY_2].action = Mainmeau_to_Task_randw;
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
	
	state_transition[Task_randw][KEY_1].state_name = Task_randw;
	state_transition[Task_randw][KEY_1].action = Pidplus;
	state_transition[Task_randw][KEY_2].state_name = Task_randw;
	state_transition[Task_randw][KEY_2].action = Pidminus;
	state_transition[Task_randw][KEY_3].state_name = Task_randw;
	state_transition[Task_randw][KEY_3].action = Pidwrite;
	state_transition[Task_randw][KEY_4].state_name = Mainmeau;
	state_transition[Task_randw][KEY_4].action = Task_randw_to_Mainmeau;
	
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
* 备    注 		   ：优先级要大于各个任务，保证按键处理实时性，增加用户体验
*******************************************************************************/
void keyhandle_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;	
	rt_uint32_t keynum;
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
		uwRet = rt_mb_recv(mb_key, &keynum, RT_WAITING_FOREVER); 	  /* 等待时间：一直 */
		if(RT_EOK == uwRet)
		{
		    rt_kprintf("thread_keyhandle：收到按键:%d,开始处理\n",keynum);
			state_transfer(current_state, keynum);
		}
		else
			rt_kprintf("thread_keyhandle：按键接收失败！\n");
    }
}
/*******************************************************************************
* 函 数 名         : state_transfer
* 函数功能		   : 状态转移函数
* 输    入         : 无
* 输    出         : 无
* 备    注 		   ：
*******************************************************************************/
void state_transfer(uint8_t statenow, uint8_t key_receive)
{
	switch(state_transition[statenow][key_receive].action){
		case NOaction:{
               // 状态变了但无动作,一般不会这样
			break;
		}
		case Mainmeau_to_Testmeau:{
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			break;
		}
		case Testmeau_to_Mainmeau:{
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			break;
		}
		case Mainmeau_to_Task_randw:{
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			rt_mb_send(mb_ctrlAt24, state_transition[statenow][key_receive].action);/*这里放通知其他任务的操作*/
			break;
		}
		case Task_randw_to_Mainmeau:{
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			/*这里放通知其他任务的操作*/
			break;
		}
		case Mainmeau_to_Task2:{
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			/*这里放通知其他任务的操作*/
			break;
		}
		case Task2_to_Mainmeau: {
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			/*这里放通知其他任务的操作*/
			break;
		}
		default:{
			rt_kprintf("状态转移错误,action:%d\n", state_transition[statenow][key_receive].action);
			break;
		}
					
	}
	key_num = 0;		
	rt_enter_critical(); // 直接关调度器，好狠
	current_state = state_transition[statenow][key_receive].state_name; //更新状态
	rt_exit_critical();
	return;
	
		
}