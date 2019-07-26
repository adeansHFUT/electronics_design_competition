#ifndef _uart_handle_H
#define _uart_handle_H
#include <rtthread.h>
extern rt_sem_t sem_uart;
extern rt_thread_t uarthandle_thread;

void uarthandle_thread_entry(void* parameter);
#endif