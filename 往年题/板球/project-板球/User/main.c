#include "board.h"
#include "rtthread.h"
#include "include.h"

//	NVIC_Configuration(); 	 
//	OLED_Init();			//oled初始化
//	OLED_Clear(); 
//	TIM2_Init(10,72000-1);  // 1/(72M/72000)=1ms, 定时器2 ，无os时的按键
//	steerFrequency_Init(1);	 //舵机初始化
/*Program Size解释: Code：代码的大小
					RO：常量所占空间
					RW：程序中已经初始化的变量所占空间
					ZI：未初始化的static和全局变量以及堆栈所占的空间
					Flash占用大小=Code+RO+RW
					SRAM占用大小=RW+ZI
*/

int my_ipc_create(void);
int my_timer_create(void);
int my_thread_create(void);
int my_thread_startup(void);
int my_other_create(void);
/*                           线程优先级排序
*************************************************************************
1：main函数线程(执行一次)
2：按键处理keyhandle
3: camera_uart处理线程
4：软件定时器-按键扫描
6：debug_uart处理线程
7：Banqiu_thread线程
8: taskreadAT24处理线程
10：display显示线程
**************************************************************************/

/*                           main 函数
**************************************************************************/
int main(void)
{	
    /* 
	 * 开发板硬件初始化，RTT系统初始化已经在main函数之前完成，
	 * 即在component.c文件中的rtthread_startup()函数中完成了。
	 * 所以在main函数中，只需要创建线程和启动线程即可。
	 */
	my_ipc_create();
	my_timer_create();
	my_thread_create();
    my_other_create();
	my_thread_startup();             
}

/* 
*************创建所有线程间通信和同步的对象**************
*/
int my_ipc_create(void)
{
	 /* 创建一个信号量 */
	sem_debug_uart = rt_sem_create("sem_debug_uart",/* 名字 */
                     0,     /* 信号量初始值 */
                     RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
    if(sem_debug_uart != RT_NULL)
		rt_kprintf("debug_uart信号量创建成功！\n\n");
	
	 /* 创建一个信号量 */
	sem_camera_uart = rt_sem_create("sem_camera_uart",/* 名字 */
                     0,     /* 信号量初始值 */
                     RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
    if(sem_camera_uart != RT_NULL)
		rt_kprintf("camera_uart信号量创建成功！\n\n");
	
	/* 创建一个信号量 */
	sem_Banqiu_task = rt_sem_create("sem_Banqiu_task",/* 名字 */
                     0,     /* 信号量初始值 */
                     RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
    if(sem_Banqiu_task != RT_NULL)
		rt_kprintf("sem_Banqiu_task信号量创建成功！\n\n");
	
	/* 创建一个按键处理邮箱 */
	mb_key = rt_mb_create("mb_key",/* 名字 */
							5,     /*邮箱大小 */
							RT_IPC_FLAG_FIFO); /* 模式 FIFO(0x00)*/
    if(mb_key != RT_NULL)
		rt_kprintf("按键处理邮箱创建成功！\n\n");
	
	/* 创建一个display邮箱 */
	mb_display = rt_mb_create("mb_display",/* 名字 */
							5,     /*邮箱大小 */
							RT_IPC_FLAG_FIFO); /* 模式 FIFO(0x00)*/
    if(mb_display != RT_NULL)
		rt_kprintf("oled显示邮箱创建成功！\n\n");
	
	/* 创建一个mb_ctrlAt24邮箱 */
	mb_ctrlAt24 = rt_mb_create("mb_ctrlAt24",/* 名字 */
							5,     /*邮箱大小 */
							RT_IPC_FLAG_FIFO); /* 模式 FIFO(0x00)*/
    if(mb_ctrlAt24 != RT_NULL)
		rt_kprintf("ctrlAt24邮箱创建成功！\n\n");
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
									50,  /* 定时器超时时间*/
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
	debug_uarthandle_thread =                          /* 线程控制块指针 */
    rt_thread_create( "debug_uarthandle",              /* 线程名字 */
                      debug_uarthandle_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      6,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
	if (debug_uarthandle_thread != RT_NULL)
		rt_kprintf("debug_uart处理线程创建成功！\n\n");
	else
		rt_kprintf("debug_uart处理线程创建失败！\n\n");
	
	camera_uarthandle_thread =                          /* 线程控制块指针 */
    rt_thread_create( "camera_uarthandle",              /* 线程名字 */
                      camera_uarthandle_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      3,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
	if (camera_uarthandle_thread != RT_NULL)
		rt_kprintf("camera_uart处理线程创建成功！\n\n");
	else
		rt_kprintf("camera_uart处理线程创建失败！\n\n");
	
	keyhandle_thread =                          /* 线程控制块指针 */
    rt_thread_create( "keyhandle",              /* 线程名字 */
                      keyhandle_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      2,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
	if (keyhandle_thread != RT_NULL)
		rt_kprintf("按键处理线程创建成功！\n\n");
	else
		rt_kprintf("按键处理线程创建失败！\n\n");
	
	taskreadAT24_thread =                          /* 线程控制块指针 */
    rt_thread_create( "taskreadAT24",              /* 线程名字 */
                      taskreadAT24_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      8,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
	if (taskreadAT24_thread != RT_NULL)
		rt_kprintf("AT24线程创建成功！\n\n");
	else
		rt_kprintf("AT24线程创建失败！\n\n");
	
	display_thread =                          /* 线程控制块指针 */
    rt_thread_create( "display",              /* 线程名字 */
                      display_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      10,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
	if (display_thread != RT_NULL)
		rt_kprintf("显示刷新线程创建成功！\n\n");
	else
		rt_kprintf("显示刷新线程创建失败！\n\n");
	
	Banqiu_thread =                          /* 线程控制块指针 */
    rt_thread_create( "Banqiu_thread",              /* 线程名字 */
                      Banqiu_set_pid_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      7,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
	if (Banqiu_thread != RT_NULL)
		rt_kprintf("Banqiu_thread线程创建成功！\n\n");
	else
		rt_kprintf("Banqiu_thread线程创建失败！\n\n");
	
	return 0;
}

/*
*******************创建其他东西********************
*/
int my_other_create(void)
{
	pid_steer1 = pos_pid_control_create(2.0, 0, 1.0);
	if(pid_steer1 != RT_NULL)
		rt_kprintf("创建PID_steer1成功！\n\n");
	else
		rt_kprintf("创建PID_steer1失败！\n\n");
	
	pid_steer2 = pos_pid_control_create(2.0, 0, 1.0);
	if(pid_steer2 != RT_NULL)
		rt_kprintf("创建PID_steer2成功！\n\n");
	else
		rt_kprintf("创建PID_steer2失败！\n\n");
	/*初始设置一波*/
	pid_steer1->maximum = +500;   // 输出的极值
    pid_steer1->minimum = -500;
    pid_steer1->anti_windup_value = 100.0f;  // 积分抗饱和值
	pid_steer1->control.sample_time = 10;   // 10 ms
	pid_steer2->maximum = +500;   // 输出的极值
    pid_steer2->minimum = -500;
    pid_steer2->anti_windup_value = 100.0f;  // 积分抗饱和值
	pid_steer2->control.sample_time = 10;   // 10 ms
	return 0;
}
 /*
*******************启动线程，开启调度*******************
*/
int my_thread_startup(void)
{
	 /* 开启调度debug_uart */
   if (debug_uarthandle_thread != RT_NULL)
   {
		rt_thread_startup(debug_uarthandle_thread); 
        rt_kprintf("debug_uart线程开始调度！\n\n");
   }	
   else
        return -1;
   
    /* 开启调度camera_uart */
   if (camera_uarthandle_thread != RT_NULL)
   {
		rt_thread_startup(camera_uarthandle_thread); 
        rt_kprintf("camera_uart线程开始调度！\n\n");
   }	
   else
        return -1;
   
   /* 开启调度按键处理线程 */
   if (keyhandle_thread != RT_NULL)
   {
		rt_thread_startup(keyhandle_thread); 
        rt_kprintf("按键处理线程开始调度！\n\n");

   }	
   else
        return -1;
   /* 开启调度显示线程 */
    if (display_thread != RT_NULL)
   {
		rt_thread_startup(display_thread); 
        rt_kprintf("显示线程开始调度！\n\n");

   }	
   else
        return -1;
   /* 开启调度AT24线程 */
    if (taskreadAT24_thread != RT_NULL)
   {
		rt_thread_startup(taskreadAT24_thread); 
        rt_kprintf("AT24线程开始调度！\n\n");

   }	
   else
        return -1;
    /* 开启调度板球线程 */
   if (Banqiu_thread != RT_NULL)
   {
		rt_thread_startup(Banqiu_thread); 
        rt_kprintf("板球线程开始调度！\n\n");

   }	
   else
        return -1;
   
   return 0;
}