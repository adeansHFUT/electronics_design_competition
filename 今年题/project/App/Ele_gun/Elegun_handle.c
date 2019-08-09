#include "Elegun_handle.h"
#include "bsp_pwm.h"
#include "bsp_steer.h"
#include "bsp_sr04.h"
#include "display.h"
#include "board.h"
#include "key_handle.h"
#include <stdlib.h>
#include <math.h>

rt_thread_t Elegun_thread = RT_NULL;
rt_thread_t Elegun_autofire_thread = RT_NULL;
rt_sem_t sem_elegun = RT_NULL;
rt_sem_t sem_elegun_autofire = RT_NULL;
uint16_t ele_distance = 200; //设定长度 
int8_t  ele_angle = 0;  // 设定角度
uint8_t receive_x = 0; // 接受到的X值
double dis_rate = 923;  // 距离转角度比例(最小600，不然无法计算反三角) 923 FOR 小炮
float dis_angle = 0; // 舵机一转动的角度
float offset_x = 0;
float offset_dead_block = 0.02; // 偏移量死区大小(0.02差不多)
float  btm_kp = 15;  // 控制舵机旋转的比例系数(初始15差不多)
int8_t  last_btm_degree = 0; // 上一次底部舵机的角度
/*******************************************************************************
* 函 数 名         : Elegun_fire_thread_entry
* 函数功能		   : 基础部分入口函数
* 输    入         : 
* 输    出         : 无
*******************************************************************************/
void Elegun_fire_thread_entry(void* parameter)  // 基础部分
{
	rt_err_t uwRet = RT_EOK;	
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
		uwRet = rt_sem_take(sem_elegun, RT_WAITING_FOREVER); 	  /* 等待时间：一直 */
		if(RT_EOK == uwRet)
		{
			//舵机转到指定位置
			dis_angle = asin(2*ele_distance/dis_rate)/2 * 180.0/3.1416;  // 反三角计算
			pwm_set_Duty(&steer1, Steer1_S3010_mid + 
			(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * dis_angle );   // 距离转角度， 上转
			if(ele_angle < 0)
				pwm_set_Duty(&steer2, Steer2_S3010_mid -
				(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (ele_angle) ) ;   // 左转
			else
				pwm_set_Duty(&steer2, Steer2_S3010_mid -
				(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (ele_angle) ) ;   // 右转
			/***发射部分***/
			rt_thread_mdelay(1200); // 延时1.2S发射			
			Fire = 1;// 发射弹丸
			Charge = 1; // 关闭充电
			rt_thread_mdelay(800); // 延时800ms(让电流流完，可控硅截至)
			Fire = 0;//发射完毕		
			Charge = 0; // 充电		
			rt_mb_send(mb_display, TO_fire_ok); // 通知显示刷新
		}	
	}
}
/*******************************************************************************
* 函 数 名         : btm_servo_control
* 函数功能		   : 底部舵机的比例控制
					这里舵机使用开环控制
* 输    入         : offset_x (缩放后，-1~1)
* 输    出         : 无
*******************************************************************************/

int8_t btm_servo_control(void)
{
	int8_t delta_degree, next_btm_degree;
	 //设置最小阈值
    if(offset_x < offset_dead_block && offset_x > -offset_dead_block)
       offset_x = 0;
 
    // offset范围在-50到50左右
    delta_degree = offset_x * btm_kp;
    // 计算得到新的底部舵机角度
    next_btm_degree = last_btm_degree + delta_degree;
    // 添加边界检测
    if(next_btm_degree < -30)
        next_btm_degree = -30;
    else if (next_btm_degree > 30)
        next_btm_degree = 30;
    last_btm_degree = next_btm_degree;
    return last_btm_degree;
}

/*******************************************************************************
* 函 数 名         : Elegun_autofire_thread_entry
* 函数功能		   : 发挥部分第一题入口函数
* 输    入         : 
* 输    出         : 无
*******************************************************************************/
void Elegun_autofire_thread_entry(void* parameter)  
{
	rt_err_t uwRet = RT_EOK;	
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
		
		uwRet = rt_sem_take(sem_elegun_autofire, RT_WAITING_FOREVER); 	  /* 等待时间：一直 */
		if(RT_EOK == uwRet)
		{
			rt_thread_mdelay(500); // 延迟一秒等数据稳定
			offset_x = ((float)receive_x/Img_width - 0.5) * 2;
			while(offset_x != 0) // 调控云台舵机2
			{
				offset_x = ((float)receive_x/Img_width - 0.5) * 2;
				ele_angle = btm_servo_control(); // 比例控制
				if(ele_angle < 0)
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (ele_angle) ) ;   // 左转
				else
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (ele_angle) ) ;   // 右转
				rt_thread_mdelay(35);  // 让舵机动到位
			}
			USART_Cmd(camera_uart_device.uart_module, DISABLE);  // 不使用摄像头了关uart接收
			ele_distance = (uint16_t)(Hcsr04GetLength()+Hcsr04GetLength())*0.5; // 获取10次
			ele_distance = ele_distance - 30;  // 引导和靶心位置是30cm
			rt_mb_send(mb_display, Wave_update); // 通知显示原始得出的距离
			if(ele_distance >= 300) // 长度限幅
				ele_distance = 300;
			else if(ele_distance <= 200 )
				ele_distance = 200;
			dis_angle = asin(2*ele_distance/dis_rate)/2 * 180.0/3.1416;  // 反三角计算
			pwm_set_Duty(&steer1, Steer1_S3010_mid + 
			(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * dis_angle );   // 距离转角度， 上转
			/***发射部分***/
			rt_thread_mdelay(1200); // 延时发射
			Fire = 1;// 发射弹丸
			Charge = 1; // 关闭充电
			rt_thread_mdelay(800); // 延时800ms
			Fire = 0;
			Charge = 0; // 开启充电
			rt_mb_send(mb_display, TO_fire_ok); // 通知显示刷新
		}
	}
}