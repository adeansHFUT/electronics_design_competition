#include "display.h"
#include "SysTick.h"
#include "rtthread.h"
#include "board.h"
#include "include.h"

/* 定义显示邮箱 */
rt_mailbox_t mb_display = RT_NULL;
/* 定义显示线程控制块 */
rt_thread_t display_thread = RT_NULL;

extern show_node pagetable[16]; //用于OLED 屏幕显示字符的节点数字
/*16字体0模式--->4个节点(8字符,4行1列)*/
/*12字体0模式--->8个节点(10字符,8行1列)*/
/*16字体1模式--->8个节点(4字符,,4行2列)*/
/*12字体1模式--->16个节点(5字符,8行2列)*/

void main_Display_init(void);
void task1_xxx_Display_init(void);
void Task_randw_display_init(void);
void Banqiu_set_pid_Display_init(void);
void Banqiu_setB_Display_init(void);
void Banqiu_setA_Display_init(void);
void steer_test_init(void);
void Pos_input_init(void);
void Steer_move_fire_init(void);
void Elegun_autofire_init(void);
void Elegun_autofire_set_init(void);
void Elegun_shakefire_set_init(void);
void Elegun_shakefire_init(void);
void Fire_ok_display(void);
/*******************************************************************************
* 函 数 名         : display_thread_entry
* 函数功能		   : 界面显示线程入口函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void display_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;	
	rt_uint32_t action;
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
		uwRet = rt_mb_recv(mb_display, &action, RT_WAITING_FOREVER); 	  /* 等待时间：一直 */
		if(RT_EOK == uwRet)
		{
		    switch(action){
/****************Mainmeau状态出发*********************/
				case Mainmeau_to_Testmeau:{
					task1_xxx_Display_init(); // 放显示更新函数
					rt_kprintf("thread_display：task1界面更新成功！\n");
					break;
				}
				case Mainmeau_to_Task_randw:{
					Task_randw_display_init();// 放显示更新函数
					break;
				}
				case Mainmeau_to_Banqiu_setA:{
					Banqiu_setA_Display_init();// 放显示更新函数
					break;
				}
				case Mainmeau_to_Steer_test:{
					steer_test_init();
					break;
				}
				case Mainmeau_to_Pos_input:{
					Pos_input_init();
					break;
				}
				case Mainmeau_to_Elegun_autofire_set:{
					Elegun_autofire_set_init();
					break;
				}
				case Mainmeau_to_Elegun_shakefire_set:{
					Elegun_shakefire_set_init();
					break;
				}
/****************Testmeau状态出发*********************/				
				case Testmeau_to_Mainmeau:{
					main_Display_init();  // 放显示更新函数
					rt_kprintf("thread_display：回到main界面更新成功！\n");
					break;
				}			
/****************Task_randw状态出发*********************/					
				case Task_randw_to_Mainmeau:{
					main_Display_init();   // 放显示更新函数
					break;
				}
				case Pidplus:case Pidminus:{
					updatepage(pagetable, 0,"pid",re_AT24, 0);
					showpage(pagetable, 1, 12);
					break;
				}
				case Pidwrite:{
					updatepage(pagetable, 1,"ok",0, 1);
					showpage(pagetable, 1, 12);
					break;
				}
/****************Banqiu_setA状态出发*********************/		
				case Banqiu_setA_plus:case Banqiu_setA_minus:{
					updatepage(pagetable, 0,"setA",targetA, 0);
					showpage(pagetable, 1, 16);
					break;
				}
				case Banqiu_setA_to_Banqiu_setB:{
					Banqiu_setB_Display_init();
					break;
				}
				case Banqiu_setA_to_Banqiu_set_pid:{
					Banqiu_set_pid_Display_init();
					break;
				}
				case Banqiu_setA_to_Mainmeau:{
					main_Display_init();
					break;
				}
/****************Banqiu_setB状态出发*********************/		
				case Banqiu_setB_plus:case Banqiu_setB_minus:{
					updatepage(pagetable, 0,"setB",targetB, 0);
					showpage(pagetable, 1, 16);
					break;
				}

				case Banqiu_setB_to_Banqiu_set_pid:{
					Banqiu_set_pid_Display_init();
					break;
				}
				case Banqiu_setB_to_Mainmeau:{
					main_Display_init();
					break;
				}
/****************Banqiu_set_pid状态出发*********************/
				case Banqiu_set_pid_to_Mainmeau:{
					updatepage(pagetable, 3,"ok!", 0, 1);
					showpage(pagetable, 1, 16);
					rt_thread_mdelay(1000);
					main_Display_init(); // 放显示更新函数
					break;
				}
				case Banqiu_P_plus:case Banqiu_P_minus:{
					updatepage(pagetable, 0,"Kp", (u16)(pid_steer1->kp*10), 0); // 乘10后再显示
					updatepage(pagetable, 6,"duo1",pid_steer1->test_out, 0);
					updatepage(pagetable, 7,"duo2",pid_steer2->test_out, 0);
					showpage(pagetable, 1, 16);
					break;
				}
				case Banqiu_I_plus:case Banqiu_I_minus:{
					updatepage(pagetable, 1,"Ki", (u16)(pid_steer1->ki*10), 0);
					showpage(pagetable, 1, 16);
					break;
				}
				case Banqiu_D_plus:case Banqiu_D_minus:{
					updatepage(pagetable, 2,"Kd", (u16)(pid_steer1->kd*10), 0);
					showpage(pagetable, 1, 16);
					break;
				}
				case Banqiu_next:{
					updatepage(pagetable, 4,"nowt", target_point[target_now].number, 0); 
					showpage(pagetable, 1, 16);
					break;
				}
/****************Steer_test状态出发*********************/
				case Steer_plus:case Steer_minus:{
					updatepage(pagetable, 0,"Pwm", test_duty, 0); 
					showpage(pagetable, 1, 16);
					break;
				}
				case Steer_test_to_Mainmeau:{
					main_Display_init();
					break;
				}
/****************Pos_input状态出发*********************/
				case Distance_plus:case Distance_minus:case Distance_plus_ten:{
					updatepage(pagetable, 0,"dis", ele_distance, 0);
					showpage(pagetable, 1, 16);
					break;
				}
				case Distance_rate_plus: case Distance_rate_minus:{
					updatepage(pagetable, 4,"rate", (uint16_t)dis_rate, 0);
					showpage(pagetable, 1, 16);
					break;
				}
				case Angle_plus:case Angle_minus:{
					if(ele_angle >= 0)
						updatepage(pagetable, 1,"+ang", ele_angle, 1); 
					else
						updatepage(pagetable, 1,"-ang", -ele_angle, 1); 
					showpage(pagetable, 1, 16);
					break;
				}
				case Pos_input_to_Mainmeau:{
					main_Display_init();
					break;
				}
				case Pos_input_to_Steer_move_fire:{
					Steer_move_fire_init();
					break;
				}
/****************Steer_move_fire状态出发*********************/
				case Steer_move_fire_to_Pos_input:{
					Pos_input_init();
					break;
				}
				case TO_fire_ok:{
					Fire_ok_display();
					break;		
				}
/****************Elegun_autofire_set状态出发*********************/
				case Elegun_autofire_set_to_Mainmeau:{
					main_Display_init();
					break;
				}
				case Autofire_Ki_plus:case Autofire_Ki_minus:{
					updatepage(pagetable, 3,"Ki", btm_ki, 0);
					showpage(pagetable, 1, 16);
					break;
				}
				case Autofire_Kp_plus: case Autofire_Kp_minus:{
					updatepage(pagetable, 2,"Kp", btm_kp, 0);
					showpage(pagetable, 1, 16);
					break;
				}
				case Elegun_autofire_set_to_Elegun_autofire:{
					Elegun_autofire_init();
					break;
				}
				case Dead_block_plus:case Dead_block_minus:{
					updatepage(pagetable, 4,"dead", offset_dead_block*1000, 0);
					showpage(pagetable, 1, 16);
					break;
				}
/****************Elegun_autofire状态出发*********************/
				case Elegun_autofire_to_Elegun_autofire_set:{
					Elegun_autofire_set_init();
					break;
				}
				case Rec_update:{  // shake fire 也可调用
					updatepage(pagetable, 3,"recx", receive_x, 0);
					showpage(pagetable, 1, 16);
					break;
				}
				case Wave_update:{
					updatepage(pagetable, 4,"wave", ele_distance, 0);
					showpage(pagetable, 1, 16);
					break;
				}
/****************Elegun_shakefire_set状态出发*********************/
				case Elegun_shakefire_set_to_Elegun_shakefire:{
					Elegun_shakefire_init();
					break;
				}
				case Advance_amount_plus:case Advance_amount_minus:{
					updatepage(pagetable, 2,"advance", shake_advance_amount, 0);
					showpage(pagetable, 0, 16);
					break;
				}
				case Elegun_shakefire_set_to_Mainmeau:{
					main_Display_init();
					break;
				}
/****************Elegun_shakefire状态出发*********************/
				case Elegun_shakefire_to_Elegun_shakefire_set:{
					Elegun_shakefire_set_init();
					break;
				}
/****************default*********************/					
				default:{
					rt_kprintf("thread_display：显示更新错误，错误action:%d\n", action);  // 打印失败码
					break;
				}
						
			}
		}
    }
}

/*******************************************************************************
* 函 数 名         : main_Display_init
* 函数功能		   : 主界面显示初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void main_Display_init(void)
{
	clearpage(pagetable);  // 清显示数组
 	OLED_Clear();  // 清屏
	updatepage(pagetable, 0,"main",0, 1);
	updatepage(pagetable, 1,"test",1, 1); 
	updatepage(pagetable, 2,"test",2, 1);
	updatepage(pagetable, 3,"gun1",3, 1);
	updatepage(pagetable, 4,"pwm",4, 1); // 更新页面数据
	updatepage(pagetable, 5,"gun2",5, 1);
	updatepage(pagetable, 6,"Kp", (uint16_t)btm_kp, 1);
	showpage(pagetable, 1, 16);
}
/*******************************************************************************
* 函 数 名         : task1_xxx_Display_init
* 函数功能		   : 界面显示初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void task1_xxx_Display_init(void)
{
	clearpage(pagetable);
	OLED_Clear();
	updatepage(pagetable, 0,"task",0, 1);
	updatepage(pagetable, 1,"one",0, 1);
	updatepage(pagetable, 2,"key_n",0, 1); 
	updatepage(pagetable, 3,"s2",2, 1);
	updatepage(pagetable, 4,"s3",3, 1);
	updatepage(pagetable, 5,"s4",4, 1); // 更新页面数据
	showpage(pagetable, 1, 12);
}
/*******************************************************************************
* 函 数 名         : Task_randw_display_init
* 函数功能		   : 界面显示初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Task_randw_display_init(void)
{
	clearpage(pagetable);
	OLED_Clear();
	updatepage(pagetable, 0, "pid", re_AT24, 1);
	showpage(pagetable, 1, 12);
}
/*******************************************************************************
* 函 数 名         : Banqiu_set_pid_Display_init
* 函数功能		   : 主界面显示初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Banqiu_setA_Display_init(void)
{
	clearpage(pagetable);  // 清显示数组
 	OLED_Clear();  // 清屏
	updatepage(pagetable, 0,"setA", targetA, 1);
	updatepage(pagetable, 4,"next",0, 1); 
	updatepage(pagetable, 5,"exit",0, 1); 
	showpage(pagetable, 1, 16);
}

void Banqiu_setB_Display_init(void)
{
	clearpage(pagetable);  // 清显示数组
 	OLED_Clear();  // 清屏
	updatepage(pagetable, 0,"setB", targetB, 1);
	updatepage(pagetable, 4,"next",0, 1); 
	updatepage(pagetable, 5,"exit",0, 1); 
	showpage(pagetable, 1, 16);
}
/*******************************************************************************
* 函 数 名         : Banqiu_set_pid_Display_init
* 函数功能		   : 主界面显示初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Banqiu_set_pid_Display_init(void)
{
	clearpage(pagetable);  // 清显示数组
 	OLED_Clear();  // 清屏
	updatepage(pagetable, 0,"Kp",(u16)(pid_steer1->kp*10), 1);
	updatepage(pagetable, 1,"Ki",(u16)(pid_steer1->ki*10), 1);
	updatepage(pagetable, 2,"Kd",(u16)(pid_steer1->kd*10), 1); 
	updatepage(pagetable, 4,"nowt",target_point[target_now].number, 1);  
	updatepage(pagetable, 5,"exit",0, 1); 
	updatepage(pagetable, 6,"duo1",pid_steer1->test_out, 1);
	updatepage(pagetable, 7,"duo2",pid_steer2->test_out, 1);
	showpage(pagetable, 1, 16);
}

/*******************************************************************************
* 函 数 名         : steer_test_init
* 函数功能		   : 舵机测试初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void steer_test_init(void)
{
	clearpage(pagetable);  // 清显示数组
 	OLED_Clear();  // 清屏
	updatepage(pagetable, 0,"Pwm", test_duty, 1);
	updatepage(pagetable, 5,"exit",0, 1); 
	showpage(pagetable, 1, 16);
}
/*******************************************************************************
* 函 数 名         : Pos_input_init
* 函数功能		   : 位置输入初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Pos_input_init(void)
{
	clearpage(pagetable);  // 清显示数组
 	OLED_Clear();  // 清屏
	updatepage(pagetable, 0,"dis", ele_distance, 1);
	if(ele_angle >= 0)
		updatepage(pagetable, 1,"+ang", ele_angle, 1); 
	else
		updatepage(pagetable, 1,"-ang", -ele_angle, 1); 
	updatepage(pagetable, 2,"fire", 3, 1);
	updatepage(pagetable, 3,"exit", 4, 1);
	updatepage(pagetable, 4,"rate", (uint16_t)dis_rate, 1);
	showpage(pagetable, 1, 16);
}
/*******************************************************************************
* 函 数 名         : Steer_move_fire_init
* 函数功能		   : 基础开火初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Steer_move_fire_init(void)
{
	clearpage(pagetable);  // 清显示数组
 	OLED_Clear();  // 清屏
	updatepage(pagetable, 0,"distance", (uint16_t)ele_distance, 1);
	updatepage(pagetable, 2,"to fire!", 0, 1);
	updatepage(pagetable, 3,"exit", 4, 1);
	showpage(pagetable, 0, 16);
}
/*******************************************************************************
* 函 数 名         : Fire_ok_display
* 函数功能		   : 开火结束显示函数（显示本次开火参数：开炮竖直角度）
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Fire_ok_display(void)
{
	clearpage(pagetable);  // 清显示数组
 	OLED_Clear();  // 清屏
	updatepage(pagetable, 0,"dis ang", (uint16_t)dis_angle, 1);
	updatepage(pagetable, 2,"fire ok!", 0, 1);
	updatepage(pagetable, 3,"exit", 4, 1);
	showpage(pagetable, 0, 16);
}
/*******************************************************************************
* 函 数 名         : Elegun_autofire_set_init
* 函数功能		   : 自动瞄准设置初始化函数(显示Kp，rate，receivex，wave)
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Elegun_autofire_set_init(void)
{
	clearpage(pagetable);  // 清显示数组
 	OLED_Clear();  // 清屏
	updatepage(pagetable, 0,"auto", 0, 1);
	updatepage(pagetable, 1,"fire", 0, 1);
	updatepage(pagetable, 2,"Kp", btm_kp, 1);
	updatepage(pagetable, 3,"Ki", btm_ki, 1);
	updatepage(pagetable, 4,"dead", offset_dead_block*1000, 1);
	updatepage(pagetable, 5,"rate", (uint16_t)dis_rate, 1);
	updatepage(pagetable, 7,"exit", 4, 1);
	showpage(pagetable, 1, 16);
}
/*******************************************************************************
* 函 数 名         : Elegun_autofire_init
* 函数功能		   : 自动瞄准初始化函数(显示Kp，rate，receivex，wave)
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Elegun_autofire_init(void)
{
	clearpage(pagetable);  // 清显示数组
 	OLED_Clear();  // 清屏
	updatepage(pagetable, 0,"auto", 0, 1);
	updatepage(pagetable, 1,"fire", 0, 1);
	updatepage(pagetable, 3,"recx", receive_x, 1);
	updatepage(pagetable, 4,"wave", ele_distance, 1);
	updatepage(pagetable, 5,"rate", (uint16_t)dis_rate, 1);
	updatepage(pagetable, 7,"exit", 4, 1);
	showpage(pagetable, 1, 16);
}
/*******************************************************************************
* 函 数 名         : Elegun_autofire_set_init
* 函数功能		   : 自动瞄准设置初始化函数(显示Kp，rate，receivex，wave)
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Elegun_shakefire_set_init(void)
{
	clearpage(pagetable);  // 清显示数组
 	OLED_Clear();  // 清屏
	updatepage(pagetable, 0,"ready", 0, 1);
	updatepage(pagetable, 1,"shake", 0, 1);
	updatepage(pagetable, 2,"advance", shake_advance_amount, 1);
	updatepage(pagetable, 3,"exit" ,0, 1);
	showpage(pagetable, 0, 16);
}
/*******************************************************************************
* 函 数 名         : Elegun_autofire_init
* 函数功能		   : 自动瞄准初始化函数(显示Kp，rate，receivex，wave)
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Elegun_shakefire_init(void)
{
	clearpage(pagetable);  // 清显示数组
 	OLED_Clear();  // 清屏
	updatepage(pagetable, 0,"to", 0, 1);
	updatepage(pagetable, 1,"fire", 0, 1);
	updatepage(pagetable, 3,"rec ", receive_x, 1);
	updatepage(pagetable, 5,"exit", 4, 1);
	showpage(pagetable, 1, 16);
}