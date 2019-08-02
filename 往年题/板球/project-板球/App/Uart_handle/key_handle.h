#ifndef _key_handle_H
#define _key_handle_H
#include "rtthread.h"
/*******************************����״̬ת������״̬����0��ʼ************************************************/
typedef enum Statename{ SameState = 0, Mainmeau, Testmeau, Task_randw, Banqiu_Task1}Statename; 
/*******************************����ת������ִ�еĶ�������0��ʼ************************************************/
typedef enum Action{NOaction = 0,
					Mainmeau_to_Testmeau, Testmeau_to_Mainmeau,  				 
					Mainmeau_to_Banqiu_Task1, Banqiu_Task1_to_Mainmeau,  
					Mainmeau_to_Task_randw, Task_randw_to_Mainmeau, Pidplus, Pidminus, Pidwrite, // pid��д��������
					Banqiu_P_plus, Banqiu_P_minus, Banqiu_I_plus, Banqiu_I_minus, Banqiu_D_plus, Banqiu_D_minus, Banqiu_pid_write  // ��������1
					
}Action;
extern Statename current_state; 
void keyhandle_thread_entry(void* parameter);
void statetable_init(void);  // �����board.c�����ʼ��
extern rt_thread_t keyhandle_thread;
#endif