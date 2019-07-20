#include "display.h"
#include "key.h"
#include "oled.h"
#include "SysTick.h"

extern u8 key_num;
extern show_node pagetable[16]; //����OLED ��Ļ��ʾ�ַ��Ľڵ�����
/*16����0ģʽ--->4���ڵ�(8�ַ�,4��1��)*/
/*12����0ģʽ--->8���ڵ�(10�ַ�,8��1��)*/
/*16����1ģʽ--->8���ڵ�(4�ַ�,,4��2��)*/
/*12����1ģʽ--->16���ڵ�(5�ַ�,8��2��)*/
void task1_xxx(void);
void main_Display_init(void);
void task1_xxx_Display_init(void);
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
	autoRefresh = 0;
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
	//showpage(pagetable, 1, 16);
	autoRefresh = 1; // �Զ�ˢ��
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