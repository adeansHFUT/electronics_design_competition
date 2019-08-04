#include "banqiu_handle.h"
#include "board.h"
#include "bsp_steer.h"
rt_sem_t sem_Banqiu_task = RT_NULL;
rt_thread_t Banqiu_thread = RT_NULL;
pos_pid_control_t pid_steer1, pid_steer2;

Banqiu_point standing_point[10];
Banqiu_point target_point[4];
Banqiu_point current_point;
u8 targetA, targetB;
u8 target_now = 0;
/*******************************************************************************
* �� �� ��         : standing_point_init
* ��������		   : �̶�������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

void standing_point_init(void)
{
	standing_point[1].number = 1;
	standing_point[1].x = 0;
	standing_point[1].y = 0;
	standing_point[2].number = 2;
	standing_point[2].x = 0;
	standing_point[2].y = 0;
	standing_point[3].number = 3;
	standing_point[3].x = 0;
	standing_point[3].y = 0;
	standing_point[4].number = 4;
	standing_point[4].x = 0;
	standing_point[4].y = 0;
	standing_point[5].number = 5;
	standing_point[5].x = 0;
	standing_point[5].y = 0;
	standing_point[6].number = 6;
	standing_point[6].x = 0;
	standing_point[6].y = 0;
	standing_point[7].number = 7;
	standing_point[7].x = 0;
	standing_point[7].y = 0;
	standing_point[8].number = 8;
	standing_point[8].x = 0;
	standing_point[8].y = 0;
	standing_point[9].number = 9;
	standing_point[9].x = 0;
	standing_point[9].y = 0;
}

/*******************************************************************************
* �� �� ��         : Banqiu_set_pid_thread_entry
* ��������		   : �����˶��߳���ں���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Banqiu_set_pid_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;	
    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
		uwRet = rt_sem_take(sem_Banqiu_task, RT_WAITING_FOREVER); 	  /* �ȴ�ʱ�䣺һֱ */
		if(RT_EOK == uwRet)
		{
			auto_control_set_target(&pid_steer1->control, target_point[target_now].y);   // ���һ����y����
			auto_control_set_target(&pid_steer2->control, target_point[target_now].x);   // ���������x����
			pid_steer1->control.update(pid_steer1, current_point.y); // ˢ��pid
			pid_steer2->control.update(pid_steer2, current_point.x);
			/* ����޷� */
			if(Steer1_S3010_mid + pid_steer1->control.output > Steer1_S3010_max)
				pwm_set_Duty(&steer1, Steer1_S3010_max);
			else if(Steer1_S3010_mid + pid_steer1->control.output < Steer1_S3010_min)
				pwm_set_Duty(&steer1, Steer1_S3010_min);
			else
				pwm_set_Duty(&steer1, Steer1_S3010_mid + pid_steer1->control.output);
			/* ����޷� */
			if(Steer2_S3010_mid + pid_steer2->control.output > Steer2_S3010_max)
				pwm_set_Duty(&steer2, Steer2_S3010_max);
			else if(Steer2_S3010_mid + pid_steer2->control.output < Steer2_S3010_min)
				pwm_set_Duty(&steer2, Steer2_S3010_min);
			else
				pwm_set_Duty(&steer2, Steer2_S3010_mid + pid_steer2->control.output);
		}	
		uwRet = rt_sem_release(sem_Banqiu_task);
    }
}