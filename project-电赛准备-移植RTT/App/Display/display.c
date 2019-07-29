#include "display.h"
#include "SysTick.h"
#include "rtthread.h"
#include "board.h"
#include "include.h"

/* ������ʾ���� */
rt_mailbox_t mb_display = RT_NULL;
/* ������ʾ�߳̿��ƿ� */
rt_thread_t display_thread = RT_NULL;

extern show_node pagetable[16]; //����OLED ��Ļ��ʾ�ַ��Ľڵ�����
/*16����0ģʽ--->4���ڵ�(8�ַ�,4��1��)*/
/*12����0ģʽ--->8���ڵ�(10�ַ�,8��1��)*/
/*16����1ģʽ--->8���ڵ�(4�ַ�,,4��2��)*/
/*12����1ģʽ--->16���ڵ�(5�ַ�,8��2��)*/

void main_Display_init(void);
void task1_xxx_Display_init(void);
void Task_randw_display_init(void);
/*******************************************************************************
* �� �� ��         : display_thread_entry
* ��������		   : ������ʾ�߳���ں���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void display_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;	
	rt_uint32_t action;
    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
		uwRet = rt_mb_recv(mb_display, &action, RT_WAITING_FOREVER); 	  /* �ȴ�ʱ�䣺һֱ */
		if(RT_EOK == uwRet)
		{
		    switch(action){
				case Mainmeau_to_Testmeau:{
					task1_xxx_Display_init(); // ����ʾ���º���
					rt_kprintf("thread_display��task1������³ɹ���\n");
					break;
				}											
				case Testmeau_to_Mainmeau:{
					main_Display_init();  // ����ʾ���º���
					rt_kprintf("thread_display���ص�main������³ɹ���\n");
					break;
				}
				case Mainmeau_to_Task_randw:{
					Task_randw_display_init();// ����ʾ���º���
					break;
				}
				case Task_randw_to_Mainmeau:{
					main_Display_init();   // ����ʾ���º���
					break;
				}
				case Mainmeau_to_Task2:{
					  // ����ʾ���º���
					break;
				}
				case Task2_to_Mainmeau:{
					  // ����ʾ���º���
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
				default:{
					rt_kprintf("thread_display����ʾ���´��󣬴���action:%d\n", action);  // ��ӡʧ����
					break;
				}
						
			}
		}
    }
}

/*******************************************************************************
* �� �� ��         : main_Display_init
* ��������		   : ��������ʾ��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void main_Display_init(void)
{
	clearpage(pagetable);  // ����ʾ����
 	OLED_Clear();  // ����
	updatepage(pagetable, 0,"main",0, 1);
	updatepage(pagetable, 1,"meau",0, 1);
	updatepage(pagetable, 2,"tk1",1, 1); 
	updatepage(pagetable, 3,"tk2",2, 1);
	updatepage(pagetable, 4,"tk3",3, 1);
	updatepage(pagetable, 5,"tk4",4, 1); // ����ҳ������
	showpage(pagetable, 1, 16);
}
/*******************************************************************************
* �� �� ��         : task1_xxx_Display_init
* ��������		   : ������ʾ��ʼ������
* ��    ��         : ��
* ��    ��         : ��
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
	updatepage(pagetable, 5,"s4",4, 1); // ����ҳ������
	showpage(pagetable, 1, 12);
}
/*******************************************************************************
* �� �� ��         : Task_randw_display_init
* ��������		   : ������ʾ��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Task_randw_display_init(void)
{
	clearpage(pagetable);
	OLED_Clear();
	updatepage(pagetable, 0, "pid", re_AT24, 1);
	showpage(pagetable, 1, 12);
}