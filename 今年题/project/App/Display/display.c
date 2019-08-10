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
/****************Mainmeau״̬����*********************/
				case Mainmeau_to_Testmeau:{
					task1_xxx_Display_init(); // ����ʾ���º���
					rt_kprintf("thread_display��task1������³ɹ���\n");
					break;
				}
				case Mainmeau_to_Task_randw:{
					Task_randw_display_init();// ����ʾ���º���
					break;
				}
				case Mainmeau_to_Banqiu_setA:{
					Banqiu_setA_Display_init();// ����ʾ���º���
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
/****************Testmeau״̬����*********************/				
				case Testmeau_to_Mainmeau:{
					main_Display_init();  // ����ʾ���º���
					rt_kprintf("thread_display���ص�main������³ɹ���\n");
					break;
				}			
/****************Task_randw״̬����*********************/					
				case Task_randw_to_Mainmeau:{
					main_Display_init();   // ����ʾ���º���
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
/****************Banqiu_setA״̬����*********************/		
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
/****************Banqiu_setB״̬����*********************/		
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
/****************Banqiu_set_pid״̬����*********************/
				case Banqiu_set_pid_to_Mainmeau:{
					updatepage(pagetable, 3,"ok!", 0, 1);
					showpage(pagetable, 1, 16);
					rt_thread_mdelay(1000);
					main_Display_init(); // ����ʾ���º���
					break;
				}
				case Banqiu_P_plus:case Banqiu_P_minus:{
					updatepage(pagetable, 0,"Kp", (u16)(pid_steer1->kp*10), 0); // ��10������ʾ
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
/****************Steer_test״̬����*********************/
				case Steer_plus:case Steer_minus:{
					updatepage(pagetable, 0,"Pwm", test_duty, 0); 
					showpage(pagetable, 1, 16);
					break;
				}
				case Steer_test_to_Mainmeau:{
					main_Display_init();
					break;
				}
/****************Pos_input״̬����*********************/
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
/****************Steer_move_fire״̬����*********************/
				case Steer_move_fire_to_Pos_input:{
					Pos_input_init();
					break;
				}
				case TO_fire_ok:{
					Fire_ok_display();
					break;		
				}
/****************Elegun_autofire_set״̬����*********************/
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
/****************Elegun_autofire״̬����*********************/
				case Elegun_autofire_to_Elegun_autofire_set:{
					Elegun_autofire_set_init();
					break;
				}
				case Rec_update:{  // shake fire Ҳ�ɵ���
					updatepage(pagetable, 3,"recx", receive_x, 0);
					showpage(pagetable, 1, 16);
					break;
				}
				case Wave_update:{
					updatepage(pagetable, 4,"wave", ele_distance, 0);
					showpage(pagetable, 1, 16);
					break;
				}
/****************Elegun_shakefire_set״̬����*********************/
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
/****************Elegun_shakefire״̬����*********************/
				case Elegun_shakefire_to_Elegun_shakefire_set:{
					Elegun_shakefire_set_init();
					break;
				}
/****************default*********************/					
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
	updatepage(pagetable, 1,"test",1, 1); 
	updatepage(pagetable, 2,"test",2, 1);
	updatepage(pagetable, 3,"gun1",3, 1);
	updatepage(pagetable, 4,"pwm",4, 1); // ����ҳ������
	updatepage(pagetable, 5,"gun2",5, 1);
	updatepage(pagetable, 6,"Kp", (uint16_t)btm_kp, 1);
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
/*******************************************************************************
* �� �� ��         : Banqiu_set_pid_Display_init
* ��������		   : ��������ʾ��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Banqiu_setA_Display_init(void)
{
	clearpage(pagetable);  // ����ʾ����
 	OLED_Clear();  // ����
	updatepage(pagetable, 0,"setA", targetA, 1);
	updatepage(pagetable, 4,"next",0, 1); 
	updatepage(pagetable, 5,"exit",0, 1); 
	showpage(pagetable, 1, 16);
}

void Banqiu_setB_Display_init(void)
{
	clearpage(pagetable);  // ����ʾ����
 	OLED_Clear();  // ����
	updatepage(pagetable, 0,"setB", targetB, 1);
	updatepage(pagetable, 4,"next",0, 1); 
	updatepage(pagetable, 5,"exit",0, 1); 
	showpage(pagetable, 1, 16);
}
/*******************************************************************************
* �� �� ��         : Banqiu_set_pid_Display_init
* ��������		   : ��������ʾ��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Banqiu_set_pid_Display_init(void)
{
	clearpage(pagetable);  // ����ʾ����
 	OLED_Clear();  // ����
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
* �� �� ��         : steer_test_init
* ��������		   : ������Գ�ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void steer_test_init(void)
{
	clearpage(pagetable);  // ����ʾ����
 	OLED_Clear();  // ����
	updatepage(pagetable, 0,"Pwm", test_duty, 1);
	updatepage(pagetable, 5,"exit",0, 1); 
	showpage(pagetable, 1, 16);
}
/*******************************************************************************
* �� �� ��         : Pos_input_init
* ��������		   : λ�������ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Pos_input_init(void)
{
	clearpage(pagetable);  // ����ʾ����
 	OLED_Clear();  // ����
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
* �� �� ��         : Steer_move_fire_init
* ��������		   : ���������ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Steer_move_fire_init(void)
{
	clearpage(pagetable);  // ����ʾ����
 	OLED_Clear();  // ����
	updatepage(pagetable, 0,"distance", (uint16_t)ele_distance, 1);
	updatepage(pagetable, 2,"to fire!", 0, 1);
	updatepage(pagetable, 3,"exit", 4, 1);
	showpage(pagetable, 0, 16);
}
/*******************************************************************************
* �� �� ��         : Fire_ok_display
* ��������		   : ���������ʾ��������ʾ���ο��������������ֱ�Ƕȣ�
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Fire_ok_display(void)
{
	clearpage(pagetable);  // ����ʾ����
 	OLED_Clear();  // ����
	updatepage(pagetable, 0,"dis ang", (uint16_t)dis_angle, 1);
	updatepage(pagetable, 2,"fire ok!", 0, 1);
	updatepage(pagetable, 3,"exit", 4, 1);
	showpage(pagetable, 0, 16);
}
/*******************************************************************************
* �� �� ��         : Elegun_autofire_set_init
* ��������		   : �Զ���׼���ó�ʼ������(��ʾKp��rate��receivex��wave)
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Elegun_autofire_set_init(void)
{
	clearpage(pagetable);  // ����ʾ����
 	OLED_Clear();  // ����
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
* �� �� ��         : Elegun_autofire_init
* ��������		   : �Զ���׼��ʼ������(��ʾKp��rate��receivex��wave)
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Elegun_autofire_init(void)
{
	clearpage(pagetable);  // ����ʾ����
 	OLED_Clear();  // ����
	updatepage(pagetable, 0,"auto", 0, 1);
	updatepage(pagetable, 1,"fire", 0, 1);
	updatepage(pagetable, 3,"recx", receive_x, 1);
	updatepage(pagetable, 4,"wave", ele_distance, 1);
	updatepage(pagetable, 5,"rate", (uint16_t)dis_rate, 1);
	updatepage(pagetable, 7,"exit", 4, 1);
	showpage(pagetable, 1, 16);
}
/*******************************************************************************
* �� �� ��         : Elegun_autofire_set_init
* ��������		   : �Զ���׼���ó�ʼ������(��ʾKp��rate��receivex��wave)
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Elegun_shakefire_set_init(void)
{
	clearpage(pagetable);  // ����ʾ����
 	OLED_Clear();  // ����
	updatepage(pagetable, 0,"ready", 0, 1);
	updatepage(pagetable, 1,"shake", 0, 1);
	updatepage(pagetable, 2,"advance", shake_advance_amount, 1);
	updatepage(pagetable, 3,"exit" ,0, 1);
	showpage(pagetable, 0, 16);
}
/*******************************************************************************
* �� �� ��         : Elegun_autofire_init
* ��������		   : �Զ���׼��ʼ������(��ʾKp��rate��receivex��wave)
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Elegun_shakefire_init(void)
{
	clearpage(pagetable);  // ����ʾ����
 	OLED_Clear();  // ����
	updatepage(pagetable, 0,"to", 0, 1);
	updatepage(pagetable, 1,"fire", 0, 1);
	updatepage(pagetable, 3,"rec ", receive_x, 1);
	updatepage(pagetable, 5,"exit", 4, 1);
	showpage(pagetable, 1, 16);
}