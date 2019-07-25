#include <rtthread.h>
#include "uart_handle.h"
#include "bsp_usart.h"
#include <string.h>

/* 定义信号量控制块 */
rt_sem_t uart_sem = RT_NULL;
/* 定义线程控制块 */
rt_thread_t uarthandle_thread = RT_NULL;

