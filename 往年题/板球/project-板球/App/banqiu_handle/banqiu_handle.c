#include "banqiu_handle.h"
#include "board.h"
#include "bsp_steer.h"
rt_sem_t sem_banqiu_task1 = RT_NULL;
pos_pid_control_t pid_steer1, pid_steer2;

/*******************************************************************************
* 函 数 名         : display_thread_entry
* 函数功能		   : 界面显示线程入口函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void banqiu_task1_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;	
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
		uwRet = rt_sem_take(sem_banqiu_task1, RT_WAITING_FOREVER); 	  /* 等待时间：一直 */
		//pid_steer1->control.update()
		
		uwRet = rt_sem_release(sem_banqiu_task1);
    }
}