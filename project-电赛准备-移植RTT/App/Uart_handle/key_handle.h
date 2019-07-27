#ifndef _key_handle_H
#define _key_handle_H
#include "rtthread.h"
/*******************************定义状态转换机的状态，从0开始************************************************/
typedef enum Statename{ Mainmeau, Testmeau, Task1, Task2}Statename; 
/*******************************定义转换过程执行的动作，从0开始************************************************/
typedef enum Action{NOaction = 0,
					Mainmeau_to_Testmeau, Testmeau_to_Mainmeau,  
					Mainmeau_to_Task1, Task1_to_Mainmeau, 
					Mainmeau_to_Task2, Task2_to_Mainmeau}Action;
extern Statename current_state; 
void keyhandle_thread_entry(void* parameter);
void statetable_init(void);  // 这个放board.c里面初始化
extern rt_thread_t keyhandle_thread;
#endif