#include <rtthread.h>
#include "uart_handle.h"
#include "bsp_usart.h"
#include <string.h>

/* �����ź������ƿ� */
rt_sem_t uart_sem = RT_NULL;
/* �����߳̿��ƿ� */
rt_thread_t uarthandle_thread = RT_NULL;

