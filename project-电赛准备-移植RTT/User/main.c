#include "board.h"
#include "rtthread.h"
#include "include.h"
//	SysTick_Init(72);
//	//LED_Init(); // PC�˿�
//	NVIC_Configuration(); 	 
//	OLED_Init();			//oled��ʼ��
//	OLED_Clear(); 
//	TIM2_Init(10,72000-1);  // 1/(72M/72000)=1ms, ��ʱ��2
//	steerFrequency_Init(1);	 //�����ʼ��
//	main_Display();
int my_ipc_create(void);
int my_thread_create(void);
int my_thread_startup(void);
/*                           main ����
*************************************************************************
*/
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
    /* 
	 * ������Ӳ����ʼ����RTTϵͳ��ʼ���Ѿ���main����֮ǰ��ɣ�
	 * ����component.c�ļ��е�rtthread_startup()����������ˡ�
	 * ������main�����У�ֻ��Ҫ�����̺߳������̼߳��ɡ�
	 */
    rt_kprintf("����һ��[Ұ��]-STM32F103-ָ����-RTT�жϹ���ʵ�飡\n");
    rt_kprintf("���ڷ������ݴ����ж�,����������!\n");
	my_ipc_create();
	my_thread_create();
	my_thread_startup();
  
 
                  
}
static void uarthandle_thread_entry(void* parameter)
{
  rt_err_t uwRet = RT_EOK;	
    /* ������һ������ѭ�������ܷ��� */
  while (1)
  {
	uwRet = rt_sem_take(uart_sem,	/* ��ȡ�����жϵ��ź��� */
                        0); 	  /* �ȴ�ʱ�䣺0 */
    if(RT_EOK == uwRet)
    {
      rt_kprintf("�յ�����:%s\n",Usart_Rx_Buf);
      memset(Usart_Rx_Buf,0,USART_RBUFF_SIZE);/* ���� */
    }
  }
}
/*************�����̼߳�ͨ�ź�ͬ���Ķ���**************/
int my_ipc_create(void)
{
	 /* ����һ���ź��� */
	uart_sem = rt_sem_create("uart_sem",/* ���� */
                     0,     /* �ź�����ʼֵ */
                     RT_IPC_FLAG_FIFO); /* �ź���ģʽ FIFO(0x00)*/
    if (uart_sem != RT_NULL)
    rt_kprintf("�ź��������ɹ���\n\n");
	return 0;
}
/********************�����߳�*********************/
int my_thread_create(void)
{
	uarthandle_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "usart",              /* �߳����� */
                      uarthandle_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      2,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
	return 0;
}
 /********************�����̣߳���������********************/
int my_thread_startup(void)
{
	 /* �����̣߳��������� */
   if (uarthandle_thread != RT_NULL)
   {
		rt_thread_startup(uarthandle_thread);
	    return 0;
   }	
   else
        return -1;
}