#include "board.h"
#include "rtthread.h"
#include "include.h"

//	NVIC_Configuration(); 	 
//	OLED_Init();			//oled��ʼ��
//	OLED_Clear(); 
//	TIM2_Init(10,72000-1);  // 1/(72M/72000)=1ms, ��ʱ��2 ����osʱ�İ���
//	steerFrequency_Init(1);	 //�����ʼ��
int my_ipc_create(void);
int my_timer_create(void);
int my_thread_create(void);
int my_thread_startup(void);
/*                           �߳����ȼ�����
*************************************************************************
1��main�����߳�
2����������keyhandle
4�������ʱ��-����ɨ��
6��uart�����߳�
8��display��ʾ�߳�
*************************************************************************
*/

/*                           main ����
*************************************************************************
*/
int main(void)
{	
    /* 
	 * ������Ӳ����ʼ����RTTϵͳ��ʼ���Ѿ���main����֮ǰ��ɣ�
	 * ����component.c�ļ��е�rtthread_startup()����������ˡ�
	 * ������main�����У�ֻ��Ҫ�����̺߳������̼߳��ɡ�
	 */
    rt_kprintf("���ڷ������ݴ����ж�,����������!\n");
	my_ipc_create();
	my_timer_create();
	my_thread_create();
	my_thread_startup();             
}
/* 
*************���������̼߳�ͨ�ź�ͬ���Ķ���**************
*/
int my_ipc_create(void)
{
	 /* ����һ���ź��� */
	sem_uart = rt_sem_create("sem_uart",/* ���� */
                     0,     /* �ź�����ʼֵ */
                     RT_IPC_FLAG_FIFO); /* �ź���ģʽ FIFO(0x00)*/
    if(sem_uart != RT_NULL)
		rt_kprintf("uart�ź��������ɹ���\n\n");
	
	/* ����һ�������������� */
	mb_key = rt_mb_create("mb_key",/* ���� */
							5,     /*�����С */
							RT_IPC_FLAG_FIFO); /* ģʽ FIFO(0x00)*/
    if(mb_key != RT_NULL)
		rt_kprintf("�����������䴴���ɹ���\n\n");
	
	/* ����һ��display���� */
	mb_display = rt_mb_create("mb_display",/* ���� */
							5,     /*�����С */
							RT_IPC_FLAG_FIFO); /* ģʽ FIFO(0x00)*/
    if(mb_display != RT_NULL)
		rt_kprintf("oled��ʾ���䴴���ɹ���\n\n");
	
	return 0;
}
/*
*******************�������������������ʱ��********************
*/
int my_timer_create(void)
{

	timer_keyscan = rt_timer_create("timer_keyscan",/* ���� */
									keyscan_callback,     /* ��ʱ���� */
									0,   /* ��ڲ��� */
									50,  /* ��ʱ����ʱʱ��*/
									RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);   /* �����ʱ��������*/
    if (timer_keyscan != RT_NULL)
	{
		rt_timer_start(timer_keyscan);
		rt_kprintf("����ɨ�趨ʱ�������ɹ���\n\n");
	}
   
	return 0;
}
/*
*******************���������߳�********************
*/
int my_thread_create(void)
{
	uarthandle_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "uarthandle",              /* �߳����� */
                      uarthandle_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      6,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
	if (uarthandle_thread != RT_NULL)
    rt_kprintf("uart�����̴߳����ɹ���\n\n");
	
	keyhandle_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "keyhandle",              /* �߳����� */
                      keyhandle_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      3,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
	if (keyhandle_thread != RT_NULL)
    rt_kprintf("���������̴߳����ɹ���\n\n");
	
	display_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "display",              /* �߳����� */
                      display_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      8,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
	if (keyhandle_thread != RT_NULL)
    rt_kprintf("��ʾˢ���̴߳����ɹ���\n\n");
	return 0;
}
 /*
*******************�����̣߳���������*******************
*/
int my_thread_startup(void)
{
	 /* �����̣߳��������� */
   if (uarthandle_thread != RT_NULL)
   {
		rt_thread_startup(uarthandle_thread); 
        rt_kprintf("�û��߳̿�ʼ���ȣ�\n\n");
	    return 0;
   }	
   else
        return -1;
}