#ifndef _key_handle_H
#define _key_handle_H
#include "rtthread.h"
/*******************************定义状态转换机的状态，从0开始************************************************/
typedef enum Statename{ SameState = 0, Mainmeau, Testmeau, Task_randw, Banqiu_Task1}Statename; 
/*******************************定义转换过程执行的动作，从0开始************************************************/
typedef enum Action{NOaction = 0,
					Mainmeau_to_Testmeau, Testmeau_to_Mainmeau,  				 
					Mainmeau_to_Banqiu_Task1, Banqiu_Task1_to_Mainmeau,  
					Mainmeau_to_Task_randw, Task_randw_to_Mainmeau, Pidplus, Pidminus, Pidwrite, // pid读写测试任务
					Banqiu_P_plus, Banqiu_P_minus, Banqiu_I_plus, Banqiu_I_minus, Banqiu_D_plus, Banqiu_D_minus, Banqiu_pid_write  // 板球任务1
					
}Action;
extern Statename current_state; 
void keyhandle_thread_entry(void* parameter);
void statetable_init(void);  // 这个放board.c里面初始化
extern rt_thread_t keyhandle_thread;
#endif