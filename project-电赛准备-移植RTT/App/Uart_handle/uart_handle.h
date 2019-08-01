#ifndef _uart_handle_H
#define _uart_handle_H
#include <rtthread.h>
extern rt_sem_t sem_debug_uart;
extern rt_thread_t debug_uarthandle_thread;

void debug_uarthandle_thread_entry(void* parameter);
#endif