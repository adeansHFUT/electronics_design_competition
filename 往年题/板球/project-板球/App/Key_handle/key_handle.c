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
state state_transition[10][10] = {0}; 
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
	state_transition[Mainmeau][KEY_3].state_name = Banqiu_Task1;
	state_transition[Mainmeau][KEY_3].action = Mainmeau_to_Banqiu_Task1;

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
		
	
	state_transition[Banqiu_Task1][KEY_5].action = Banqiu_P_plus;
	state_transition[Banqiu_Task1][KEY_6].action = Banqiu_P_minus;
	state_transition[Banqiu_Task1][KEY_7].action = Banqiu_I_plus;
	state_transition[Banqiu_Task1][KEY_8].action = Banqiu_I_minus;
	state_transition[Banqiu_Task1][KEY_1].action = Banqiu_D_plus;
	state_transition[Banqiu_Task1][KEY_2].action = Banqiu_D_minus;
	state_transition[Banqiu_Task1][KEY_3].action = Banqiu_pid_write;
	state_transition[Banqiu_Task1][KEY_4].state_name = Mainmeau;
	state_transition[Banqiu_Task1][KEY_4].action = Banqiu_Task1_to_Mainmeau;
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
* 备    注 		   ：整个系统的按键事件识别中转都在这里
*******************************************************************************/
void state_transfer(uint8_t statenow, uint8_t key_receive)
{
	switch(state_transition[statenow][key_receive].action){
		case NOaction:{
               // 状态变了但无动作,一般不会这样
			break;
		}
/****************Mainmeau状态出发*********************/
		case Mainmeau_to_Testmeau:{
			rt_mb_send(mb_display, Mainmeau_to_Testmeau);
			break;
		}
		case Mainmeau_to_Task_randw:{
			rt_mb_send(mb_ctrlAt24, Mainmeau_to_Task_randw);/*通知AT24读pid*/
			rt_mb_send(mb_display, Mainmeau_to_Task_randw);
			break;
		}
		case Mainmeau_to_Banqiu_Task1:{
			USART_Cmd(camera_uart_device.uart_module, ENABLE);  // 打开uart接收
			rt_sem_release(sem_banqiu_task1);  // 启动板球task1线程
			rt_mb_send(mb_ctrlAt24, Mainmeau_to_Banqiu_Task1); /*通知AT24读pid*/
			rt_mb_send(mb_display, Mainmeau_to_Banqiu_Task1); // 通知显示刷新	
			break;
		}
/****************Testmeau状态出发*********************/
		case Testmeau_to_Mainmeau:{
			rt_mb_send(mb_display, Testmeau_to_Mainmeau);
			break;
		}
/****************Task_randw状态出发*********************/		
		case Task_randw_to_Mainmeau:{
			rt_mb_send(mb_display, Task_randw_to_Mainmeau);
			break;
		}
		case Pidplus:case Pidminus:case Pidwrite:{
			rt_mb_send(mb_ctrlAt24, state_transition[statenow][key_receive].action);
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			break;
		}
/****************Banqiu_Task1状态出发*********************/		
		case Banqiu_Task1_to_Mainmeau: {
			USART_Cmd(camera_uart_device.uart_module, DISABLE);  // 关闭uart接收
			rt_sem_take(sem_banqiu_task1, RT_WAITING_FOREVER); // 将板球任务1暂停下来(尝试)
			rt_mb_send(mb_display, Banqiu_Task1_to_Mainmeau);  
			break;
		}
		case Banqiu_P_plus:{
			pos_pid_control_set_kp(pid_steer1, pid_steer1->kp + 0.1);
			pos_pid_control_set_kp(pid_steer2, pid_steer2->kp + 0.1);
			rt_mb_send(mb_display, Banqiu_P_plus);
			break;
		}
		case Banqiu_P_minus:{
			pos_pid_control_set_kp(pid_steer1, pid_steer1->kp - 0.1);
			pos_pid_control_set_kp(pid_steer2, pid_steer2->kp - 0.1);
			rt_mb_send(mb_display, Banqiu_P_minus);
			break;
		}
		case Banqiu_I_plus:{
			pos_pid_control_set_ki(pid_steer1, pid_steer1->ki + 0.1);
			pos_pid_control_set_ki(pid_steer2, pid_steer2->ki + 0.1);
			rt_mb_send(mb_display, Banqiu_I_plus);
			break;
		}
		case Banqiu_I_minus:{
			pos_pid_control_set_ki(pid_steer1, pid_steer1->ki - 0.1);
			pos_pid_control_set_ki(pid_steer2, pid_steer2->ki - 0.1);
			rt_mb_send(mb_display, Banqiu_I_minus);
			break;
		}
		case Banqiu_D_plus:{
			pos_pid_control_set_kd(pid_steer1, pid_steer1->kd + 0.1);
			pos_pid_control_set_kd(pid_steer2, pid_steer2->kd + 0.1);
			rt_mb_send(mb_display, Banqiu_D_plus);
			break;
		}
		case Banqiu_D_minus:{
			pos_pid_control_set_kd(pid_steer1, pid_steer1->kd - 0.1);
			pos_pid_control_set_kd(pid_steer2, pid_steer2->kd - 0.1);
			rt_mb_send(mb_display, Banqiu_D_minus);
			break;
		}
		case Banqiu_pid_write:{
			rt_mb_send(mb_ctrlAt24, Banqiu_pid_write);
			rt_mb_send(mb_display, Banqiu_pid_write);
			break;
		}
/****************default*********************/	
		default:{
			rt_kprintf("状态转移错误,action:%d\n", state_transition[statenow][key_receive].action);
			break;
		}
					
	}
	if(state_transition[statenow][key_receive].state_name != SameState)	
	{
		rt_enter_critical(); // 直接关调度器，好狠
		current_state = state_transition[statenow][key_receive].state_name; //更新状态
		rt_exit_critical();
	}		
	key_num = 0;
	return;
	
		
}