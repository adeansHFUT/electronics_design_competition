#include <rtthread.h>
#include "uart_handle.h"
#include "bsp_usart.h"
#include <string.h>

/* 定义信号量控制块 */
rt_sem_t sem_debug_uart = RT_NULL;
/* 定义线程控制块 */
rt_thread_t debug_uarthandle_thread = RT_NULL;

void debug_uarthandle_thread_entry(void* parameter)
{
  rt_err_t uwRet = RT_EOK;	
    /* 任务都是一个无限循环，不能返回 */
  while (1)
  {
	uwRet = rt_sem_take(sem_debug_uart,	/* 获取串口中断的信号量 */
                        RT_WAITING_FOREVER); 	  /* 等待时间：一直 */
    if(RT_EOK == uwRet)
    {
      rt_kprintf("收到数据:%s\n",Usart_Rx_Buf);
      memset(Usart_Rx_Buf,0, DEBUG_USART_RBUFF_SIZE);/* 清零 */
    }
  }
}