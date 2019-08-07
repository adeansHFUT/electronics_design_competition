#ifndef _key_handle_H
#define _key_handle_H
#include "rtthread.h"
/*******************************定义状态转换机的状态，从0开始************************************************/
typedef enum Statename{ SameState = 0, Mainmeau, Testmeau, Task_randw,
Banqiu_set_pid,Banqiu_setA,Banqiu_setB,Steer_test,
Pos_input, Steer_move_fire,
Elegun_autofire, 
}Statename; 
/*******************************定义转换过程执行的动作，从0开始************************************************/
typedef enum Action{
NOaction = 0,
Mainmeau_to_Testmeau,Testmeau_to_Mainmeau,  				 
Mainmeau_to_Task_randw, Task_randw_to_Mainmeau, Pidplus, Pidminus, Pidwrite, // pid读写测试任务
/***************************板球任务***************************/
Mainmeau_to_Banqiu_setA,Banqiu_setA_to_Mainmeau,Banqiu_setA_to_Banqiu_setB,
Banqiu_setA_to_Banqiu_set_pid,Banqiu_setA_plus,Banqiu_setA_minus,
Banqiu_setB_to_Banqiu_setC, Banqiu_setB_to_Banqiu_set_pid, Banqiu_setB_to_Mainmeau,
Banqiu_setB_plus, Banqiu_setB_minus,
Banqiu_set_pid_to_Mainmeau, Banqiu_P_plus, Banqiu_P_minus, Banqiu_I_plus, Banqiu_I_minus, 
Banqiu_D_plus, Banqiu_D_minus, Banqiu_next, 
/***************************测试舵机任务***************************/		
Mainmeau_to_Steer_test, Steer_plus, Steer_minus, Steer_test_to_Mainmeau, 
/**************************电磁炮任务(基础部分)***************************/		
Mainmeau_to_Pos_input, Distance_plus, Distance_plus_ten, Distance_minus, Angle_plus, Angle_minus, Pos_input_to_Mainmeau,
Pos_input_to_Steer_move_fire, Steer_move_fire_to_Pos_input,  	
/**************************电磁炮任务(发挥部分1)***************************/		
Mainmeau_to_Elegun_autofire, Elegun_autofire_to_Mainmeau, Main_Kp_plus, Main_Kp_minus, Rec_update, 
}Action;
extern Statename current_state; 
void keyhandle_thread_entry(void* parameter);
void statetable_init(void);  // 这个放board.c里面初始化
extern rt_thread_t keyhandle_thread;

extern uint16_t test_duty;
#endif