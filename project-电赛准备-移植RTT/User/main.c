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
int my_thread_create(void);
int my_thread_startup(void);
/*                           main 函数
*************************************************************************
*/
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
    /* 
	 * 开发板硬件初始化，RTT系统初始化已经在main函数之前完成，
	 * 即在component.c文件中的rtthread_startup()函数中完成了。
	 * 所以在main函数中，只需要创建线程和启动线程即可。
	 */
    rt_kprintf("这是一个[野火]-STM32F103-指南者-RTT中断管理实验！\n");
    rt_kprintf("串口发送数据触发中断,任务处理数据!\n");
	my_ipc_create();
	my_thread_create();
	my_thread_startup();
  
 
                  
}
static void uarthandle_thread_entry(void* parameter)
{
  rt_err_t uwRet = RT_EOK;	
    /* 任务都是一个无限循环，不能返回 */
  while (1)
  {
	uwRet = rt_sem_take(uart_sem,	/* 获取串口中断的信号量 */
                        0); 	  /* 等待时间：0 */
    if(RT_EOK == uwRet)
    {
      rt_kprintf("收到数据:%s\n",Usart_Rx_Buf);
      memset(Usart_Rx_Buf,0,USART_RBUFF_SIZE);/* 清零 */
    }
  }
}
/*************创建线程间通信和同步的对象**************/
int my_ipc_create(void)
{
	 /* 创建一个信号量 */
	uart_sem = rt_sem_create("uart_sem",/* 名字 */
                     0,     /* 信号量初始值 */
                     RT_IPC_FLAG_FIFO); /* 信号量模式 FIFO(0x00)*/
    if (uart_sem != RT_NULL)
    rt_kprintf("信号量创建成功！\n\n");
	return 0;
}
/********************创建线程*********************/
int my_thread_create(void)
{
	uarthandle_thread =                          /* 线程控制块指针 */
    rt_thread_create( "usart",              /* 线程名字 */
                      uarthandle_thread_entry,   /* 线程入口函数 */
                      RT_NULL,             /* 线程入口函数参数 */
                      512,                 /* 线程栈大小 */
                      2,                   /* 线程的优先级 */
                      20);                 /* 线程时间片 */
	return 0;
}
 /********************启动线程，开启调度********************/
int my_thread_startup(void)
{
	 /* 启动线程，开启调度 */
   if (uarthandle_thread != RT_NULL)
   {
		rt_thread_startup(uarthandle_thread);
	    return 0;
   }	
   else
        return -1;
}