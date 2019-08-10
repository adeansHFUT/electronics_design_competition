#ifndef _Ele_gun_H
#define _Ele_gun_H
#include "rtthread.h"

#define Img_width 320
extern rt_sem_t sem_elegun;
extern uint16_t ele_distance;
extern int8_t  ele_angle;
extern rt_thread_t Elegun_thread;
extern rt_thread_t Elegun_autofire_thread;
extern rt_thread_t Elegun_shakefire_thread;
extern rt_sem_t sem_elegun_autofire;
extern rt_sem_t sem_elegun_shakefire;
extern uint8_t receive_x; // 接受到的X值
extern float  btm_kp;  // 控制舵机旋转的比例系数
extern float  btm_ki;  // 控制舵机旋转的比例系数(初始15差不多)
extern int8_t  last_btm_degree; // 上一次底部舵机的角度
extern double dis_rate;  // 距离转角度比例
extern float dis_angle; // 舵机一转动的角度
extern uint8_t elegun_shakefire_rotation; // 当前向哪个方向摇
extern uint8_t shake_advance_amount; // 提前量
extern float offset_dead_block; // 偏移量死区大小(0.02差不多)
void Elegun_fire_thread_entry(void* parameter);
void Elegun_autofire_thread_entry(void* parameter);  
void Elegun_shakefire_thread_entry(void* parameter);  
#endif