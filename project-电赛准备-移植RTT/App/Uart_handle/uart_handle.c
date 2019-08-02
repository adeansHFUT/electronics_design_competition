#include <rtthread.h>
#include "uart_handle.h"
#include "bsp_usart.h"
#include <string.h>

/* �����ź������ƿ� */
rt_sem_t sem_debug_uart = RT_NULL;
rt_sem_t sem_camera_uart = RT_NULL;
/* �����߳̿��ƿ� */
rt_thread_t debug_uarthandle_thread = RT_NULL;

void debug_uarthandle_thread_entry(void* parameter)
{
  rt_err_t uwRet = RT_EOK;	
    /* ������һ������ѭ�������ܷ��� */
  while (1)
  {
	uwRet = rt_sem_take(sem_debug_uart,	/* ��ȡ�����жϵ��ź��� */
                        RT_WAITING_FOREVER); 	  /* �ȴ�ʱ�䣺һֱ */
    if(RT_EOK == uwRet)
    {
      rt_kprintf("�յ�����:%s\n",debug_uart_device.Usart_Rx_Buf);
      memset(debug_uart_device.Usart_Rx_Buf, 0, USART_RBUFF_SIZE);/* ���� */
    }
  }
}

void camera_uarthandle_thread_entry(void* parameter)
{
  rt_err_t uwRet = RT_EOK;	
    /* ������һ������ѭ�������ܷ��� */
  while (1)
  {
	uwRet = rt_sem_take(sem_camera_uart,	/* ��ȡ�����жϵ��ź��� */
                        RT_WAITING_FOREVER); 	  /* �ȴ�ʱ�䣺һֱ */
    if(RT_EOK == uwRet)
    {
      rt_kprintf("�յ�����:%s\n",camera_uart_device.Usart_Rx_Buf);
      memset(camera_uart_device.Usart_Rx_Buf, 0, USART_RBUFF_SIZE);/* ���� */
    }
  }
}