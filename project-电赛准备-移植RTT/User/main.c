#include "board.h"
#include "rtthread.h"
#include "include.h"
//	SysTick_Init(72);
//	//LED_Init(); // PC端口
//	NVIC_Configuration(); 	 
//	OLED_Init();			//oled初始化
//	OLED_Clear(); 
//	TIM2_Init(10,72000-1);  // 1/(72M/72000)=1ms, 定时器2
//	steerFrequency_Init(1);	 //舵机初始化
//	main_Display();
int my_ipc_create(void);
int my_timer_create(void);
int my_thread_create(void);
int my_thread_startup(void);
/*                           main 函数
*************************************************************************
*/
int main(void)
{	
    /* 
	 * 开发板硬件初始化，RTT系统初始化已经在main函数之前完成，
	 * 即在component.c文件中的rtthread_startup()函数中完成了。
	 * 所以在main函数中，只需要创建线程和启动线程即可。
	 */
    rt_kprintf("串口发送数据触发中断,任务处理数据!\n");
	my_ipc_create();
	my_timer_create();
	my_thread_create();
	my_thread_startup();             
}
/* 
*************创建所有线程间通信和同步的对象**************
*/
int my_ipc_create(void)
{
	 /* 创建一个信号量 */
	sem_uart = rt_sem_create("sem_uart",/* 名字 */
                     0,     /* 信号量初始值 */
                     RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
    if(sem_uart != RT_NULL)
		rt_kprintf("uart信号量创建成功！\n\n");
	
	/* 创建一个邮箱 */
	mb_key = rt_mb_create("mb_key",/* 名字 */
							5,     /*邮箱大小 */
							RT_IPC_FLAG_FIFO); /* 模式 FIFO(0x00)*/
    if(mb_key != RT_NULL)
		rt_kprintf("按键邮箱创建成功！\n\n");
	return 0;
}
/*
*******************创建所有软件定时器********************
*/
int my_timer_create(void)
{

	timer_keyscan = rt_timer_create("timer_keyscan",/* 名字 */
									keyscan_callback,     /* 超时函数 */
									0,   /* 入口参数 */
									50,  /* 50个tick扫描一次*/
									RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);   /* 软件定时器，周期*/
    if (timer_keyscan != RT_NULL)
	{
		rt_timer_start(timer_keyscan);
		rt_kprintf("按键扫描定时器启动成功！\n\n");
	}
   
	return 0;
}
/*
*******************创建所有线程********************
*/
int my_thread_create(void)
{
	uarthandle_thread =                          /* 线程控制块指针 */
    rt_thread_create( "usart",              /* 线程名字 */
                      uarthandle_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      12,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
	if (uarthandle_thread != RT_NULL)
    rt_kprintf("线程创建成功！\n\n");
	return 0;
}
 /*
*******************启动线程，开启调度*******************
*/
int my_thread_startup(void)
{
	 /* 启动线程，开启调度 */
   if (uarthandle_thread != RT_NULL)
   {
		rt_thread_startup(uarthandle_thread); 
        rt_kprintf("用户线程开始调度！\n\n");
	    return 0;
   }	
   else
        return -1;
}