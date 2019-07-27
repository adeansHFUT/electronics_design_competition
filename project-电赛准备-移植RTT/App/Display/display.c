#include "display.h"
#include "bsp_key.h"
#include "bsp_oled.h"
#include "SysTick.h"
#include "rtthread.h"
#include "key_handle.h"

/* 定义显示邮箱 */
rt_mailbox_t mb_display = RT_NULL;
/* 定义显示线程控制块 */
rt_thread_t display_thread = RT_NULL;

extern show_node pagetable[16]; //用于OLED 屏幕显示字符的节点数字
/*16字体0模式--->4个节点(8字符,4行1列)*/
/*12字体0模式--->8个节点(10字符,8行1列)*/
/*16字体1模式--->8个节点(4字符,,4行2列)*/
/*12字体1模式--->16个节点(5字符,8行2列)*/
void task1_xxx(void);
void main_Display_init(void);
void task1_xxx_Display_init(void);
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
				case Mainmeau_to_Testmeau: task1_xxx_Display_init(); // 放显示更新函数
											key_num = 0;            // 处理完成将按键值置零
											rt_kprintf("task1界面更新成功！\n");
											break;
				case Testmeau_to_Mainmeau: main_Display_init();  // 放显示更新函数
											key_num = 0;         // 处理完成将按键值置零
											rt_kprintf("回到main界面更新成功！\n");
											break;
				case Mainmeau_to_Task1: 						// 放显示更新函数
										key_num = 0;   
										break;
				case Task1_to_Mainmeau:
										key_num = 0;
										break;
				case Mainmeau_to_Task2: 						// 放显示更新函数
										key_num = 0;   
										break;
				case Task2_to_Mainmeau: 						// 放显示更新函数
										key_num = 0;   
										break;
				case NOaction:									// 无动作不更新
								key_num = 0;
								break;
				default: 
						rt_kprintf("显示更新错误，错误action:%d\n", action);  // 打印失败码
						break;
			}
		}
    }
}
/*******************************************************************************
* 函 数 名         : main_Display
* 函数功能		   : 主界面显示函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void main_Display(void)
{
	main_Display_init();
	while(1)
	{
		switch(key_num){
			case 0: 
				break;
			case KEY_1: 
				key_num = 0;
				task1_xxx();  // 调用子界面函数
				main_Display_init();
				break;
			case KEY_2: 
				delay_ms(500);
				key_num = 0;
				break;
			case KEY_3: 
				delay_ms(500);
				key_num = 0;
				break;
			case KEY_4: 
				delay_ms(500);
				key_num = 0;
				break;
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
	clearpage(pagetable);
	updatepage(pagetable, 0,"main",0, 1);
	updatepage(pagetable, 1,"meau",0, 1);
	updatepage(pagetable, 2,"tk1",1, 1); 
	updatepage(pagetable, 3,"tk2",2, 1);
	updatepage(pagetable, 4,"tk3",3, 1);
	updatepage(pagetable, 5,"tk4",4, 1); // 更新页面数据
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
	updatepage(pagetable, 0,"task",0, 1);
	updatepage(pagetable, 1,"one",0, 1);
	updatepage(pagetable, 2,"key_n",0, 1); 
	updatepage(pagetable, 3,"s2",2, 1);
	updatepage(pagetable, 4,"s3",3, 1);
	updatepage(pagetable, 5,"s4",4, 1); // 更新页面数据
	showpage(pagetable, 1, 16);
}
/*******************************************************************************
* 函 数 名         : task1_xxx
* 函数功能		   : 界面显示函数(显示按键值)
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void task1_xxx(void)
{
	task1_xxx_Display_init();
	while(1)
	{

		switch(key_num){
			case KEY_4:
				key_num = 0;
				return;
			case 0:
				break;
			case KEY_1:	
				updatepage(pagetable, 2, "key_n", key_num, 0); // 只更新key_num这个数字	
				delay_ms(1000);
				key_num = 0;
				break;
			case KEY_2:
				updatepage(pagetable, 2, "key_n", key_num, 0); // 只更新key_num这个数字	
				delay_ms(1000);
				key_num = 0;
				break;
			case KEY_3:
				updatepage(pagetable, 2, "key_n", key_num, 0); // 只更新key_num这个数字	
				delay_ms(1000);
				key_num = 0;
				break;
		}
			
	}
}