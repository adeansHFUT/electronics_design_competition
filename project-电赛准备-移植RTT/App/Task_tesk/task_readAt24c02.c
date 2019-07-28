#include "task_readAT24c02.h"
#include "bsp_24cxx.h"
#include "rtthread.h"
#include "key_handle.h"
/* 定义task线程控制块 */
rt_thread_t taskreadAT24_thread = RT_NULL;
/* 定义task邮箱控制块 */
rt_mailbox_t mb_ctrlAt24 = RT_NULL;  // 控制AT24
uint8_t re_AT24;  // 定义AT24返回值（全局变量）

void taskreadAT24_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;
	rt_uint32_t AT_flag;
	while(AT24CXX_Check())  //检测AT24C02是否正常
	{
		rt_kprintf("AT24C02检测不正常!\n");
		rt_thread_mdelay(500);
	}
	rt_kprintf("AT24C02检测正常!\n");
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
		uwRet = rt_mb_recv(mb_ctrlAt24, &AT_flag, RT_WAITING_FOREVER);	  /* 等待时间：一直 */
		if(RT_EOK == uwRet)
		{
		    rt_kprintf("taskreadAT24_thread：收到邮箱");
			switch(AT_flag){
				case Pidread:{
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
			}
			
		}
		else
			rt_kprintf("taskreadAT24_thread：邮箱接收失败！\n");
    }
}