#ifndef _task_readAT24c02_H
#define _task_readAT24c02_H
#include "rtthread.h"
/* 定义task线程控制块 */
extern rt_thread_t taskreadAT24_thread;
/* 定义task邮箱控制块 */
extern rt_mailbox_t mb_ctrlAt24;
extern uint8_t re_AT24;  // 定义AT24返回值（全局变量）
void taskreadAT24_thread_entry(void* parameter);
#endif