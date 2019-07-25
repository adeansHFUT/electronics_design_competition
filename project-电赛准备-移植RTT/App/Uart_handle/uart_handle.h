#ifndef _uart_handle_H
#define _uart_handle_H
#include <rtthread.h>
//void uarthandle_thread_entry(void* parameter);
extern rt_sem_t uart_sem;
extern rt_thread_t uarthandle_thread;

#endif