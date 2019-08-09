#ifndef _Ele_gun_H
#define _Ele_gun_H
#include "rtthread.h"

#define Img_width 320
extern rt_sem_t sem_elegun;
extern uint16_t ele_distance;
extern int8_t  ele_angle;
extern rt_thread_t Elegun_thread;
extern rt_thread_t Elegun_autofire_thread;
extern rt_sem_t sem_elegun_autofire;
extern uint8_t receive_x; // ���ܵ���Xֵ
extern float  btm_kp;  // ���ƶ����ת�ı���ϵ��
extern int8_t  last_btm_degree; // ��һ�εײ�����ĽǶ�
extern double dis_rate;  // ����ת�Ƕȱ���
extern float dis_angle; // ���һת���ĽǶ�
void Elegun_fire_thread_entry(void* parameter);
void Elegun_autofire_thread_entry(void* parameter);  
#endif