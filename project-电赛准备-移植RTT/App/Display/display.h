#ifndef _display_H
#define _display_H
#include "rtthread.h"

void task1_xxx(void);
void main_Display(void);
extern rt_mailbox_t mb_display;
extern rt_thread_t display_thread;
void display_thread_entry(void* parameter);
#endif