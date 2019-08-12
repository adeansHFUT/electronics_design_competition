#include <rtthread.h>
#include "uart_handle.h"
#include "bsp_usart.h"
#include <string.h>
#include "include.h"
/* �����ź������ƿ� */
rt_sem_t sem_debug_uart = RT_NULL;
rt_sem_t sem_camera_uart = RT_NULL;
/* �����߳̿��ƿ� */
rt_thread_t debug_uarthandle_thread = RT_NULL;
rt_thread_t camera_uarthandle_thread = RT_NULL;

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
      rt_kprintf("�յ�����:%s\n",debug_uart_device.usart_Rx_Buf);
      memset(debug_uart_device.usart_Rx_Buf, 0, debug_uart_device.buff_size);/* ���� */
    }
  }
}
void openmv_receive(uint8_t* data)
{
	
/*ͨ�Ÿ�ʽ 0xAA 0x55 data1 data2 checkout 0x54*/	
/*����checkout=(data1+data2)�Ͱ�λ  ���� data1=0xe1,data2=0xf3,data1+data2=0x1d4,��checkout=0xd4*/
	if(data[0] == 0xAA && data[1] == 0x55 && data[4] == (u8)(data[2]+data[3]) && data[5] == 0x54)
	{
		current_point.x = data[2];
		current_point.y = data[3];
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
		openmv_receive(camera_uart_device.usart_Rx_8_buf); // ��֤��ʽ�����µ�ǰλ��
		memset(camera_uart_device.usart_Rx_8_buf, 0, camera_uart_device.buff_size);/* ���� */
    }
  }
}