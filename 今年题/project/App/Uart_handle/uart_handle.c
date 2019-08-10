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
	
/*ͨ�Ÿ�ʽ 0xAA 0x55 data1  0x54*/	
/*����checkout=(data1+data2)�Ͱ�λ  ���� data1=0xe1,data2=0xf3,data1+data2=0x1d4,��checkout=0xd4*/
	if(data[0] == 0xAA && data[1] == 0x55 && data[3] == 0x54)
	{
		receive_x = data[2];			
		rt_mb_send(mb_display, Rec_update);
		// ����ҡ������ǰshake_advance_amount����
		if(elegun_shakefire_rotation == 2 && (160-shake_advance_amount) == receive_x)  
		{
			USART_Cmd(camera_uart_device.uart_module, DISABLE);  // ��uart����
			Fire = 1;// ���䵯��
			Charge = 1; // �رճ��
			rt_thread_mdelay(800); // ��ʱ800ms
			Fire = 0;
			Charge = 0; // �������
			rt_mb_send(mb_display, TO_fire_ok); // ֪ͨ��ʾˢ��
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
		openmv_receive(camera_uart_device.usart_Rx_8_buf); // ��֤��ʽ�����µ�ǰλ��
		memset(camera_uart_device.usart_Rx_8_buf, 0, camera_uart_device.buff_size);/* ���� */
    }
  }
}