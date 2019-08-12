#include "task_readAT24c02.h"
#include "bsp_24cxx.h"
#include "rtthread.h"
#include "key_handle.h"
#include "banqiu_handle.h"
#include "pos_pid_control.h"
#include "board.h"
#include "include.h"
/* ����task�߳̿��ƿ� */
rt_thread_t taskreadAT24_thread = RT_NULL;
/* ����task������ƿ� */
rt_mailbox_t mb_ctrlAt24 = RT_NULL;  // ����AT24
uint8_t re_AT24;  // ����AT24����ֵ��ȫ�ֱ�����
uint8_t using_at24 = 1;

void taskreadAT24_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;
	rt_uint32_t AT_flag;
	while(AT24CXX_Check())  //���AT24C02�Ƿ�����
	{
		rt_kprintf("AT24C02��ⲻ����!\n");
		rt_thread_mdelay(500);
	}
	rt_kprintf("AT24C02�������!\n");
    /* ������һ������ѭ�������ܷ��� */
//	AT24CXX_WriteOneByte(8, (uint16_t)(dis_rate_small)/100);
//	AT24CXX_WriteOneByte(9, (uint16_t)(dis_rate_small)%100);
//	AT24CXX_WriteOneByte(10, (uint16_t)(dis_rate_big)/100);
//	AT24CXX_WriteOneByte(11, (uint16_t)(dis_rate_big)%100);
//	AT24CXX_WriteOneByte(12, (uint8_t)btm_kp);
//	AT24CXX_WriteOneByte(13, (uint8_t)btm_ki);
//	AT24CXX_WriteOneByte(14, (uint8_t)(offset_dead_block*1000));
//	AT24CXX_WriteOneByte(15, (uint8_t)shake_advance_amount);
//	AT24CXX_WriteOneByte(16, (uint8_t)pi_sample_time);
	
    while (1)
    {
		uwRet = rt_mb_recv(mb_ctrlAt24, &AT_flag, RT_WAITING_FOREVER);	  /* �ȴ�ʱ�䣺һֱ */
		if(RT_EOK == uwRet && using_at24 == 1)
		{
		    rt_kprintf("taskreadAT24_thread���յ�����\n");
			switch(AT_flag){
				case Mainmeau_to_Task_randw:{
					re_AT24 = AT24CXX_ReadOneByte(0);
					break;
				}
				case Pidplus:{
					re_AT24++;
					break;
				}
				case Pidminus:{
					re_AT24--;
					break;
				}
				case Pidwrite:{
					AT24CXX_WriteOneByte(0, re_AT24);
					break;
				}
    /*********************�������***************************/
//				case Banqiu_setA_to_Banqiu_set_pid:case Banqiu_setB_to_Banqiu_set_pid:{
//					u8 read_p, read_i, read_d;
//					read_p = AT24CXX_ReadOneByte(1);
//					read_i = AT24CXX_ReadOneByte(2);
//					read_d = AT24CXX_ReadOneByte(3);
//				    pos_pid_control_set_kp(pid_steer1, read_p*0.1);
//					pos_pid_control_set_kp(pid_steer2, read_p*0.1);
//					pos_pid_control_set_ki(pid_steer1, read_i*0.1);
//					pos_pid_control_set_ki(pid_steer2, read_i*0.1);
//					pos_pid_control_set_kd(pid_steer1, read_d*0.1);
//					pos_pid_control_set_kd(pid_steer2, read_d*0.1);
//					break;
//				}
//				case Banqiu_set_pid_to_Mainmeau:{
//					AT24CXX_WriteOneByte(1, (u8)(pid_steer1->kp*10));
//					AT24CXX_WriteOneByte(2, (u8)(pid_steer1->ki*10));
//					AT24CXX_WriteOneByte(3, (u8)(pid_steer1->kd*10));
//					break;
//				}
   /*********************��������***************************/
				case Mainmeau_to_Pos_input:{
					dis_rate_big = AT24CXX_ReadOneByte(10)*100 + AT24CXX_ReadOneByte(11); // ���λ����ʮλ��
					dis_rate_small = AT24CXX_ReadOneByte(8)*100 + AT24CXX_ReadOneByte(9); // ���λ����ʮλ��
					break;
				}
				case Pos_input_to_Mainmeau:{
					AT24CXX_WriteOneByte(10, (uint16_t)(dis_rate_big)/100);
					AT24CXX_WriteOneByte(11, (uint16_t)(dis_rate_big)%100);
					AT24CXX_WriteOneByte(8, (uint16_t)(dis_rate_small)/100);
					AT24CXX_WriteOneByte(9, (uint16_t)(dis_rate_small)%100);
					break;
				}
				case Mainmeau_to_Elegun_autofire_set:{
					dis_rate_big = AT24CXX_ReadOneByte(10)*100 + AT24CXX_ReadOneByte(11); // ���λ����ʮλ��
					dis_rate_small = AT24CXX_ReadOneByte(8)*100 + AT24CXX_ReadOneByte(9); // ���λ����ʮλ��
					btm_kp = (float)AT24CXX_ReadOneByte(12);
					btm_ki = (float)AT24CXX_ReadOneByte(13);
					offset_dead_block = (float)AT24CXX_ReadOneByte(14)/1000;
					pi_sample_time = AT24CXX_ReadOneByte(16);
					break;
				}
				case Elegun_autofire_set_to_Mainmeau:{
					AT24CXX_WriteOneByte(12, (uint8_t)btm_kp);
					AT24CXX_WriteOneByte(13, (uint8_t)btm_ki);
					AT24CXX_WriteOneByte(14, (uint8_t)(offset_dead_block*1000));
					break;
				}
				case Mainmeau_to_Elegun_shakefire_set:{
					dis_rate_small = AT24CXX_ReadOneByte(8)*100 + AT24CXX_ReadOneByte(9); // ���λ����ʮλ��
					shake_advance_amount = AT24CXX_ReadOneByte(15);
					pi_sample_time = AT24CXX_ReadOneByte(16);
					break;
				}
				case Elegun_shakefire_set_to_Mainmeau:{
					AT24CXX_WriteOneByte(15, (uint8_t)shake_advance_amount);
					AT24CXX_WriteOneByte(16, (uint8_t)pi_sample_time);
					break;
				}
			}
			
		}
		else
			rt_kprintf("taskreadAT24_thread���������ʧ�ܣ�\n");
    }
}