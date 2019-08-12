#ifndef _Ele_gun_H
#define _Ele_gun_H
#include "rtthread.h"

#define Img_width 320
extern rt_sem_t sem_elegun;
extern uint16_t ele_distance;
extern float  ele_angle;
extern rt_thread_t Elegun_thread;
extern rt_thread_t Elegun_autofire_thread;
extern rt_thread_t Elegun_shakefire_thread;
extern rt_sem_t sem_elegun_autofire;
extern rt_sem_t sem_elegun_shakefire;
extern uint8_t receive_x; // ���ܵ���Xֵ
extern float  btm_kp;  // ���ƶ����ת�ı���ϵ��
extern float  btm_ki;  // ���ƶ����ת�ı���ϵ��(��ʼ15���)
extern float  last_btm_degree; // ��һ�εײ�����ĽǶ�
extern double dis_rate_big ;  // ����ת�Ƕȱ���(��С600����Ȼ�޷����㷴����) 923 FOR С��  1140(һ����Ĵ�Ƕ�)
extern double dis_rate_small; // ������С�Ƕ�
extern float dis_angle; // ���һת���ĽǶ�
extern uint8_t elegun_shakefire_rotation; // ��ǰ���ĸ�����ҡ
extern uint8_t shake_advance_amount; // ��ǰ��
extern float offset_dead_block; // ƫ����������С(0.02���)
extern uint8_t pi_sample_time;  // ���piȡ��ʱ��
extern uint8_t using_big_angle; // ��һ�����Ƿ�ʹ�ô�Ƕ����
void Elegun_fire_thread_entry(void* parameter);
void Elegun_autofire_thread_entry(void* parameter);  
void Elegun_shakefire_thread_entry(void* parameter);  
#endif