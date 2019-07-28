#ifndef _key_handle_H
#define _key_handle_H
#include "rtthread.h"
/*******************************����״̬ת������״̬����0��ʼ************************************************/
typedef enum Statename{ Mainmeau, Testmeau, Task_randw, Task2}Statename; 
/*******************************����ת������ִ�еĶ�������0��ʼ************************************************/
typedef enum Action{NOaction = 0,
					Mainmeau_to_Testmeau, Testmeau_to_Mainmeau,  
					Mainmeau_to_Task_randw, Task_randw_to_Mainmeau, 
					Mainmeau_to_Task2, Task2_to_Mainmeau,
					Pidplus, Pidminus, Pidwrite, Pidread = Mainmeau_to_Task_randw // ��Task_randw�Ͷ�e2prom
					
}Action;
extern Statename current_state; 
void keyhandle_thread_entry(void* parameter);
void statetable_init(void);  // �����board.c�����ʼ��
extern rt_thread_t keyhandle_thread;
#endif