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
	updatepage(pagetable, 1,"meau",0, 1);
	updatepage(pagetable, 2,"tt1",1, 1); 
	updatepage(pagetable, 3,"tt2",2, 1);
	updatepage(pagetable, 4,"tk1",3, 1);
	updatepage(pagetable, 5,"tk2",4, 1); // 更新页面数据
	showpage(pagetable, 1, 12);
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