#ifndef _task_readAT24c02_H
#define _task_readAT24c02_H
#include "rtthread.h"
/* ����task�߳̿��ƿ� */
extern rt_thread_t taskreadAT24_thread;
/* ����task������ƿ� */
extern rt_mailbox_t mb_ctrlAt24;
extern uint8_t re_AT24;  // ����AT24����ֵ��ȫ�ֱ�����
void taskreadAT24_thread_entry(void* parameter);
#endif