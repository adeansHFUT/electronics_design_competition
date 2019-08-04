#ifndef _banqiu_handle_H
#define _banqiu_handle_H
#include "rtthread.h"
#include "board.h"
#include "sys.h"

typedef struct{
	int16_t x;
	int16_t y;
	uint8_t number;  // ±êºÅ
}Banqiu_point;

extern rt_sem_t sem_Banqiu_task;
extern pos_pid_control_t pid_steer1, pid_steer2;
extern Banqiu_point standing_point[10];
extern Banqiu_point target_point[4];
extern Banqiu_point current_point;
extern u8 targetA, targetB;
extern u8 target_now;
extern rt_thread_t Banqiu_thread;

void Banqiu_set_pid_thread_entry(void* parameter);
void standing_point_init(void);
#endif