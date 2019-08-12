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
rt_thread_t Elegun_shakefire_thread = RT_NULL;
rt_sem_t sem_elegun = RT_NULL;
rt_sem_t sem_elegun_autofire = RT_NULL;
rt_sem_t sem_elegun_shakefire = RT_NULL;
uint16_t ele_distance = 200; //设定射击距离 
float  ele_angle = 0;  // 设定角度(左右)
uint8_t receive_x = 0; // 接受到摄像头的X值
double dis_rate_big = 1140;  // 距离转角度比例(最小600，不然无法计算反三角) 923 FOR 小炮  1140(一二题的大角度)
double dis_rate_small = 842; // 第三题小角度
float dis_angle = 0; // 舵机一转动的角度(俯仰)
float offset_x = 0;  // 红色引导和摄像头中心的x偏差
float offset_dead_block = 0.018; // 偏移量死区大小(0.02差不多)
float  btm_kp = 17;  // 控制舵机旋转的比例系数(初始15差不多)
float  btm_ki = 24;  // 控制舵机旋转的积分系数()
float  last_btm_degree = 0; // 上一次底部舵机的角度
uint8_t elegun_shakefire_rotation = 0; // 当前向哪个方向摇
uint8_t shake_advance_amount = 5; // 发射提前量（和摇的速度有关）
uint8_t pi_sample_time = 35;  // 舵机pi取样时间
uint8_t using_big_angle = 0; // 第一二题是否使用大角度射击(默认大)
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
/////////////////上下转////////////////////////////					
			if(using_big_angle == 1)
			{
				dis_angle = asin(2*ele_distance/(dis_rate_big))/2 * 180.0/3.1416;  // 反三角计算
				pwm_set_Duty(&steer1, Steer1_S3010_mid + 
				(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * (90-dis_angle) + 0.5 );   // 距离转角度， 上转
			}
			else if(using_big_angle == 0)
			{	
				float fuzzy_rate = -0.0024*ele_distance*ele_distance + 0.2490*ele_distance + dis_rate_small;  //拟合结果
				dis_angle = asin(2*ele_distance/(fuzzy_rate))/2 * 180.0/3.1416;  // 反三角计算
				pwm_set_Duty(&steer1, Steer1_S3010_mid + 
				(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * (dis_angle) + 0.5 );   // 距离转角度， 上转
			}
				
/////////////////左右转////////////////////////////			
			if(ele_angle < 0)
				pwm_set_Duty(&steer2, Steer2_S3010_mid -
				(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (ele_angle) + 0.5  ) ;   // 左转
			else
				pwm_set_Duty(&steer2, Steer2_S3010_mid -
				(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (ele_angle) + 0.5 ) ;   // 右转
			/***发射部分***/
			rt_thread_mdelay(2200); // 延时1.2S发射	（让炮管里的炮稳定下来）		
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
	static float integral = 0;
	static float last_offset = 0;
	float delta_degree, next_btm_degree;
	 //设置最小阈值
    if(offset_x < offset_dead_block && offset_x > -offset_dead_block)
       offset_x = 0;
	//integral += offset_x;  // 得到积分
//	if(integral > 2)  // 积分限幅
//		integral = 2;
//	else if(integral < -2)
//		integral = -2;
    // offset范围在-1到1，改进方法一：下面相当于模糊p(打偏差小的不行)
//	if(offset_x < 0.033 && offset_x > -0.033)
//		delta_degree = offset_x * (btm_kp+15);
//	else if(offset_x < 0.25 && offset_x > -0.25)
//		delta_degree = offset_x * (btm_kp-5);
//	else 
//		delta_degree = offset_x * btm_kp;
	
	delta_degree = (offset_x-last_offset) * btm_kp + last_offset * btm_ki; // 改进方法二：加入Ki
    // 计算得到新的底部舵机角度
    next_btm_degree = last_btm_degree + delta_degree;
    // 添加边界检测
    if(next_btm_degree < -30)
        next_btm_degree = -30;
    else if (next_btm_degree > 30)
        next_btm_degree = 30;
    last_btm_degree = next_btm_degree;
	last_offset = offset_x;
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
				ele_angle = btm_servo_control(); // 比例控制
				if(ele_angle < 0)
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (ele_angle) + 0.5 ) ;   // 左转
				else
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (ele_angle) + 0.5 ) ;   // 右转
				rt_thread_mdelay(pi_sample_time);  // 让舵机动到位
				if(offset_x)
					offset_x = ((float)receive_x/Img_width - 0.5) * 2;
			}
			USART_Cmd(camera_uart_device.uart_module, DISABLE);  // 不使用摄像头了关uart接收
			rt_thread_mdelay(1500); //延迟1.5s让超声波稳定下来再测距
			ele_distance = (uint16_t)(Hcsr04GetLength()+Hcsr04GetLength())*0.5; // 获取10次
			ele_distance = ele_distance - 30;  // 引导和靶心位置是30cm
			rt_mb_send(mb_display, Wave_update); // 通知显示原始得出的距离
			if(ele_distance >= 300) // 长度限幅
				ele_distance = 300;
			else if(ele_distance <= 200 )
				ele_distance = 200;
			////////////////上下转///////////////////////
			if(using_big_angle == 1)
			{
				dis_angle = asin(2*ele_distance/(dis_rate_big))/2 * 180.0/3.1416;  // 反三角计算
				pwm_set_Duty(&steer1, Steer1_S3010_mid + 
				(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * (90-dis_angle) + 0.5 );   // 距离转角度， 上转
			}
			else if(using_big_angle == 0)
			{
				float fuzzy_rate = -0.0024*ele_distance*ele_distance + 0.2490*ele_distance + dis_rate_small;  //拟合结果
				dis_angle = asin(2*ele_distance/(fuzzy_rate))/2 * 180.0/3.1416;  // 反三角计算
				pwm_set_Duty(&steer1, Steer1_S3010_mid + 
				(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * (dis_angle) + 0.5 );   // 距离转角度， 上转
			}
			/***发射部分***/
			rt_thread_mdelay(2200); // 延时发射（让炮管里的炮稳定下来）
			Fire = 1;// 发射弹丸
			Charge = 1; // 关闭充电
			rt_thread_mdelay(800); // 延时800ms
			Fire = 0;
			Charge = 0; // 开启充电
			rt_mb_send(mb_display, TO_fire_ok); // 通知显示刷新
		}
	}
}

/*******************************************************************************
* 函 数 名         : Elegun_shakefire_thread_entry
* 函数功能		   : 发挥部分第二题入口函数
* 输    入         : 
* 输    出         : 无
*******************************************************************************/
void Elegun_shakefire_thread_entry(void* parameter)  
{
	rt_err_t uwRet = RT_EOK;	
    /* 任务都是一个无限循环，不能返回 */
    while (1)
    {
		
		uwRet = rt_sem_take(sem_elegun_shakefire, RT_WAITING_FOREVER); 	  /* 等待时间：一直 */
		if(RT_EOK == uwRet)
		{
			float steer2_tempangle = -30;
			float fuzzy_rate = 0;
			ele_distance = 250; // 直接更新要打得距离为250cm
			////////////上下转，第三题不管如何都用小角度/////////////////
			fuzzy_rate = -0.0024*ele_distance*ele_distance + 0.2490*ele_distance + dis_rate_small;  //拟合结果
			dis_angle = asin(2*ele_distance/(fuzzy_rate))/2 * 180.0/3.1416;  // 反三角计算
//			dis_angle = asin(2*ele_distance/dis_rate_small)/2 * 180.0/3.1416;  // 反三角计算
			pwm_set_Duty(&steer1, Steer1_S3010_mid + 
			(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * dis_angle + 0.5 );   // 先转动俯仰
			
			elegun_shakefire_rotation = 1; // 当前向右摇
			while(steer2_tempangle <=30)
			{
				if(steer2_tempangle < 0)
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (steer2_tempangle) + 0.5 ) ;   // 左转
				else
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (steer2_tempangle) + 0.5 ) ;   // 右转
				rt_thread_mdelay(50); // 转到另一侧
				steer2_tempangle++;
			}
			elegun_shakefire_rotation = 2; // 当前向右摇
			while(steer2_tempangle >= -30)
			{
				if(steer2_tempangle < 0)
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (steer2_tempangle) + 0.5 ) ;   // 左转
				else
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (steer2_tempangle) + 0.5 ) ;   // 右转
				rt_thread_mdelay(50); // 转到另一侧
				steer2_tempangle--;
			}
			elegun_shakefire_rotation = 0; // 之后没摇
		}
	}
}
	

