#include "banqiu_handle.h"
#include "board.h"
#include "bsp_steer.h"
rt_sem_t sem_banqiu_task1 = RT_NULL;
pos_pid_control_t pid_steer1, pid_steer2;

/*******************************************************************************
* �� �� ��         : display_thread_entry
* ��������		   : ������ʾ�߳���ں���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void banqiu_task1_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;	
    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
		uwRet = rt_sem_take(sem_banqiu_task1, RT_WAITING_FOREVER); 	  /* �ȴ�ʱ�䣺һֱ */
		//pid_steer1->control.update()
		
		uwRet = rt_sem_release(sem_banqiu_task1);
    }
}