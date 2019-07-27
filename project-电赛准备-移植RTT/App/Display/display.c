#include "display.h"
#include "bsp_key.h"
#include "bsp_oled.h"
#include "SysTick.h"
#include "rtthread.h"
#include "key_handle.h"

/* ������ʾ���� */
rt_mailbox_t mb_display = RT_NULL;
/* ������ʾ�߳̿��ƿ� */
rt_thread_t display_thread = RT_NULL;

extern show_node pagetable[16]; //����OLED ��Ļ��ʾ�ַ��Ľڵ�����
/*16����0ģʽ--->4���ڵ�(8�ַ�,4��1��)*/
/*12����0ģʽ--->8���ڵ�(10�ַ�,8��1��)*/
/*16����1ģʽ--->8���ڵ�(4�ַ�,,4��2��)*/
/*12����1ģʽ--->16���ڵ�(5�ַ�,8��2��)*/
void task1_xxx(void);
void main_Display_init(void);
void task1_xxx_Display_init(void);
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
				case Mainmeau_to_Testmeau: task1_xxx_Display_init(); // ����ʾ���º���
											key_num = 0;            // ������ɽ�����ֵ����
											rt_kprintf("task1������³ɹ���\n");
											break;
				case Testmeau_to_Mainmeau: main_Display_init();  // ����ʾ���º���
											key_num = 0;         // ������ɽ�����ֵ����
											rt_kprintf("�ص�main������³ɹ���\n");
											break;
				case Mainmeau_to_Task1: 						// ����ʾ���º���
										key_num = 0;   
										break;
				case Task1_to_Mainmeau:
										key_num = 0;
										break;
				case Mainmeau_to_Task2: 						// ����ʾ���º���
										key_num = 0;   
										break;
				case Task2_to_Mainmeau: 						// ����ʾ���º���
										key_num = 0;   
										break;
				case NOaction:									// �޶���������
								key_num = 0;
								break;
				default: 
						rt_kprintf("��ʾ���´��󣬴���action:%d\n", action);  // ��ӡʧ����
						break;
			}
		}
    }
}
/*******************************************************************************
* �� �� ��         : main_Display
* ��������		   : ��������ʾ����
* ��    ��         : ��
* ��    ��         : ��
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
				task1_xxx();  // �����ӽ��溯��
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
* �� �� ��         : main_Display_init
* ��������		   : ��������ʾ��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void main_Display_init(void)
{
	clearpage(pagetable);
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
	updatepage(pagetable, 0,"task",0, 1);
	updatepage(pagetable, 1,"one",0, 1);
	updatepage(pagetable, 2,"key_n",0, 1); 
	updatepage(pagetable, 3,"s2",2, 1);
	updatepage(pagetable, 4,"s3",3, 1);
	updatepage(pagetable, 5,"s4",4, 1); // ����ҳ������
	showpage(pagetable, 1, 16);
}
/*******************************************************************************
* �� �� ��         : task1_xxx
* ��������		   : ������ʾ����(��ʾ����ֵ)
* ��    ��         : ��
* ��    ��         : ��
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
				updatepage(pagetable, 2, "key_n", key_num, 0); // ֻ����key_num�������	
				delay_ms(1000);
				key_num = 0;
				break;
			case KEY_2:
				updatepage(pagetable, 2, "key_n", key_num, 0); // ֻ����key_num�������	
				delay_ms(1000);
				key_num = 0;
				break;
			case KEY_3:
				updatepage(pagetable, 2, "key_n", key_num, 0); // ֻ����key_num�������	
				delay_ms(1000);
				key_num = 0;
				break;
		}
			
	}
}