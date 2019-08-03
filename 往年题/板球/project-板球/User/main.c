#include "board.h"
#include "rtthread.h"
#include "include.h"

//	NVIC_Configuration(); 	 
//	OLED_Init();			//oled��ʼ��
//	OLED_Clear(); 
//	TIM2_Init(10,72000-1);  // 1/(72M/72000)=1ms, ��ʱ��2 ����osʱ�İ���
//	steerFrequency_Init(1);	 //�����ʼ��
/*Program Size����: Code������Ĵ�С
					RO��������ռ�ռ�
					RW���������Ѿ���ʼ���ı�����ռ�ռ�
					ZI��δ��ʼ����static��ȫ�ֱ����Լ���ջ��ռ�Ŀռ�
					Flashռ�ô�С=Code+RO+RW
					SRAMռ�ô�С=RW+ZI
*/

int my_ipc_create(void);
int my_timer_create(void);
int my_thread_create(void);
int my_thread_startup(void);
int my_other_create(void);
/*                           �߳����ȼ�����
*************************************************************************
1��main�����߳�(ִ��һ��)
2����������keyhandle
3: camera_uart�����߳�
4�������ʱ��-����ɨ��
6��debug_uart�����߳�
7��Banqiu_thread�߳�
8: taskreadAT24�����߳�
10��display��ʾ�߳�
**************************************************************************/

/*                           main ����
**************************************************************************/
int main(void)
{	
    /* 
	 * ������Ӳ����ʼ����RTTϵͳ��ʼ���Ѿ���main����֮ǰ��ɣ�
	 * ����component.c�ļ��е�rtthread_startup()����������ˡ�
	 * ������main�����У�ֻ��Ҫ�����̺߳������̼߳��ɡ�
	 */
	my_ipc_create();
	my_timer_create();
	my_thread_create();
    my_other_create();
	my_thread_startup();             
}

/* 
*************���������̼߳�ͨ�ź�ͬ���Ķ���**************
*/
int my_ipc_create(void)
{
	 /* ����һ���ź��� */
	sem_debug_uart = rt_sem_create("sem_debug_uart",/* ���� */
                     0,     /* �ź�����ʼֵ */
                     RT_IPC_FLAG_FIFO); /* �ź���ģʽ FIFO(0x00)*/
    if(sem_debug_uart != RT_NULL)
		rt_kprintf("debug_uart�ź��������ɹ���\n\n");
	
	 /* ����һ���ź��� */
	sem_camera_uart = rt_sem_create("sem_camera_uart",/* ���� */
                     0,     /* �ź�����ʼֵ */
                     RT_IPC_FLAG_FIFO); /* �ź���ģʽ FIFO(0x00)*/
    if(sem_camera_uart != RT_NULL)
		rt_kprintf("camera_uart�ź��������ɹ���\n\n");
	
	/* ����һ���ź��� */
	sem_Banqiu_task = rt_sem_create("sem_Banqiu_task",/* ���� */
                     0,     /* �ź�����ʼֵ */
                     RT_IPC_FLAG_FIFO); /* �ź���ģʽ FIFO(0x00)*/
    if(sem_Banqiu_task != RT_NULL)
		rt_kprintf("sem_Banqiu_task�ź��������ɹ���\n\n");
	
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
	
	/* ����һ��mb_ctrlAt24���� */
	mb_ctrlAt24 = rt_mb_create("mb_ctrlAt24",/* ���� */
							5,     /*�����С */
							RT_IPC_FLAG_FIFO); /* ģʽ FIFO(0x00)*/
    if(mb_ctrlAt24 != RT_NULL)
		rt_kprintf("ctrlAt24���䴴���ɹ���\n\n");
	return 0;
}
/*
*******************�������������ʱ��********************
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
	debug_uarthandle_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "debug_uarthandle",              /* �߳����� */
                      debug_uarthandle_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      6,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
	if (debug_uarthandle_thread != RT_NULL)
		rt_kprintf("debug_uart�����̴߳����ɹ���\n\n");
	else
		rt_kprintf("debug_uart�����̴߳���ʧ�ܣ�\n\n");
	
	camera_uarthandle_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "camera_uarthandle",              /* �߳����� */
                      camera_uarthandle_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      3,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
	if (camera_uarthandle_thread != RT_NULL)
		rt_kprintf("camera_uart�����̴߳����ɹ���\n\n");
	else
		rt_kprintf("camera_uart�����̴߳���ʧ�ܣ�\n\n");
	
	keyhandle_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "keyhandle",              /* �߳����� */
                      keyhandle_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      2,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
	if (keyhandle_thread != RT_NULL)
		rt_kprintf("���������̴߳����ɹ���\n\n");
	else
		rt_kprintf("���������̴߳���ʧ�ܣ�\n\n");
	
	taskreadAT24_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "taskreadAT24",              /* �߳����� */
                      taskreadAT24_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      8,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
	if (taskreadAT24_thread != RT_NULL)
		rt_kprintf("AT24�̴߳����ɹ���\n\n");
	else
		rt_kprintf("AT24�̴߳���ʧ�ܣ�\n\n");
	
	display_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "display",              /* �߳����� */
                      display_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      10,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
	if (display_thread != RT_NULL)
		rt_kprintf("��ʾˢ���̴߳����ɹ���\n\n");
	else
		rt_kprintf("��ʾˢ���̴߳���ʧ�ܣ�\n\n");
	
	Banqiu_thread =                          /* �߳̿��ƿ�ָ�� */
    rt_thread_create( "Banqiu_thread",              /* �߳����� */
                      Banqiu_set_pid_thread_entry,   /* �߳���ں��� */
                      RT_NULL,             /* �߳���ں������� */
                      512,                 /* �߳�ջ��С */
                      7,                   /* �̵߳����ȼ� */
                      20);                 /* �߳�ʱ��Ƭ */
	if (Banqiu_thread != RT_NULL)
		rt_kprintf("Banqiu_thread�̴߳����ɹ���\n\n");
	else
		rt_kprintf("Banqiu_thread�̴߳���ʧ�ܣ�\n\n");
	
	return 0;
}

/*
*******************������������********************
*/
int my_other_create(void)
{
	pid_steer1 = pos_pid_control_create(2.0, 0, 1.0);
	if(pid_steer1 != RT_NULL)
		rt_kprintf("����PID_steer1�ɹ���\n\n");
	else
		rt_kprintf("����PID_steer1ʧ�ܣ�\n\n");
	
	pid_steer2 = pos_pid_control_create(2.0, 0, 1.0);
	if(pid_steer2 != RT_NULL)
		rt_kprintf("����PID_steer2�ɹ���\n\n");
	else
		rt_kprintf("����PID_steer2ʧ�ܣ�\n\n");
	/*��ʼ����һ��*/
	pid_steer1->maximum = +500;   // ����ļ�ֵ
    pid_steer1->minimum = -500;
    pid_steer1->anti_windup_value = 100.0f;  // ���ֿ�����ֵ
	pid_steer1->control.sample_time = 10;   // 10 ms
	pid_steer2->maximum = +500;   // ����ļ�ֵ
    pid_steer2->minimum = -500;
    pid_steer2->anti_windup_value = 100.0f;  // ���ֿ�����ֵ
	pid_steer2->control.sample_time = 10;   // 10 ms
	return 0;
}
 /*
*******************�����̣߳���������*******************
*/
int my_thread_startup(void)
{
	 /* ��������debug_uart */
   if (debug_uarthandle_thread != RT_NULL)
   {
		rt_thread_startup(debug_uarthandle_thread); 
        rt_kprintf("debug_uart�߳̿�ʼ���ȣ�\n\n");
   }	
   else
        return -1;
   
    /* ��������camera_uart */
   if (camera_uarthandle_thread != RT_NULL)
   {
		rt_thread_startup(camera_uarthandle_thread); 
        rt_kprintf("camera_uart�߳̿�ʼ���ȣ�\n\n");
   }	
   else
        return -1;
   
   /* �������Ȱ��������߳� */
   if (keyhandle_thread != RT_NULL)
   {
		rt_thread_startup(keyhandle_thread); 
        rt_kprintf("���������߳̿�ʼ���ȣ�\n\n");

   }	
   else
        return -1;
   /* ����������ʾ�߳� */
    if (display_thread != RT_NULL)
   {
		rt_thread_startup(display_thread); 
        rt_kprintf("��ʾ�߳̿�ʼ���ȣ�\n\n");

   }	
   else
        return -1;
   /* ��������AT24�߳� */
    if (taskreadAT24_thread != RT_NULL)
   {
		rt_thread_startup(taskreadAT24_thread); 
        rt_kprintf("AT24�߳̿�ʼ���ȣ�\n\n");

   }	
   else
        return -1;
    /* �������Ȱ����߳� */
   if (Banqiu_thread != RT_NULL)
   {
		rt_thread_startup(Banqiu_thread); 
        rt_kprintf("�����߳̿�ʼ���ȣ�\n\n");

   }	
   else
        return -1;
   
   return 0;
}