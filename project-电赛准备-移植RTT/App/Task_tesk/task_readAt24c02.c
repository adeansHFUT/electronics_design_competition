#include "task_readAT24c02.h"
#include "bsp_24cxx.h"
#include "rtthread.h"
#include "key_handle.h"
/* ����task�߳̿��ƿ� */
rt_thread_t taskreadAT24_thread = RT_NULL;
/* ����task������ƿ� */
rt_mailbox_t mb_ctrlAt24 = RT_NULL;  // ����AT24
uint8_t re_AT24;  // ����AT24����ֵ��ȫ�ֱ�����

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
    while (1)
    {
		uwRet = rt_mb_recv(mb_ctrlAt24, &AT_flag, RT_WAITING_FOREVER);	  /* �ȴ�ʱ�䣺һֱ */
		if(RT_EOK == uwRet)
		{
		    rt_kprintf("taskreadAT24_thread���յ�����");
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
			rt_kprintf("taskreadAT24_thread���������ʧ�ܣ�\n");
    }
}