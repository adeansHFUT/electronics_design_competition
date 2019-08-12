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
uint16_t ele_distance = 200; //�趨������� 
float  ele_angle = 0;  // �趨�Ƕ�(����)
uint8_t receive_x = 0; // ���ܵ�����ͷ��Xֵ
double dis_rate_big = 1140;  // ����ת�Ƕȱ���(��С600����Ȼ�޷����㷴����) 923 FOR С��  1140(һ����Ĵ�Ƕ�)
double dis_rate_small = 842; // ������С�Ƕ�
float dis_angle = 0; // ���һת���ĽǶ�(����)
float offset_x = 0;  // ��ɫ����������ͷ���ĵ�xƫ��
float offset_dead_block = 0.018; // ƫ����������С(0.02���)
float  btm_kp = 17;  // ���ƶ����ת�ı���ϵ��(��ʼ15���)
float  btm_ki = 24;  // ���ƶ����ת�Ļ���ϵ��()
float  last_btm_degree = 0; // ��һ�εײ�����ĽǶ�
uint8_t elegun_shakefire_rotation = 0; // ��ǰ���ĸ�����ҡ
uint8_t shake_advance_amount = 5; // ������ǰ������ҡ���ٶ��йأ�
uint8_t pi_sample_time = 35;  // ���piȡ��ʱ��
uint8_t using_big_angle = 0; // ��һ�����Ƿ�ʹ�ô�Ƕ����(Ĭ�ϴ�)
/*******************************************************************************
* �� �� ��         : Elegun_fire_thread_entry
* ��������		   : ����������ں���
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void Elegun_fire_thread_entry(void* parameter)  // ��������
{
	rt_err_t uwRet = RT_EOK;	
    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
		uwRet = rt_sem_take(sem_elegun, RT_WAITING_FOREVER); 	  /* �ȴ�ʱ�䣺һֱ */
		if(RT_EOK == uwRet)
		{
/////////////////����ת////////////////////////////					
			if(using_big_angle == 1)
			{
				dis_angle = asin(2*ele_distance/(dis_rate_big))/2 * 180.0/3.1416;  // �����Ǽ���
				pwm_set_Duty(&steer1, Steer1_S3010_mid + 
				(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * (90-dis_angle) + 0.5 );   // ����ת�Ƕȣ� ��ת
			}
			else if(using_big_angle == 0)
			{	
				float fuzzy_rate = -0.0024*ele_distance*ele_distance + 0.2490*ele_distance + dis_rate_small;  //��Ͻ��
				dis_angle = asin(2*ele_distance/(fuzzy_rate))/2 * 180.0/3.1416;  // �����Ǽ���
				pwm_set_Duty(&steer1, Steer1_S3010_mid + 
				(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * (dis_angle) + 0.5 );   // ����ת�Ƕȣ� ��ת
			}
				
/////////////////����ת////////////////////////////			
			if(ele_angle < 0)
				pwm_set_Duty(&steer2, Steer2_S3010_mid -
				(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (ele_angle) + 0.5  ) ;   // ��ת
			else
				pwm_set_Duty(&steer2, Steer2_S3010_mid -
				(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (ele_angle) + 0.5 ) ;   // ��ת
			/***���䲿��***/
			rt_thread_mdelay(2200); // ��ʱ1.2S����	�����ڹ�������ȶ�������		
			Fire = 1;// ���䵯��
			Charge = 1; // �رճ��
			rt_thread_mdelay(800); // ��ʱ800ms(�õ������꣬�ɿع����)
			Fire = 0;//�������		
			Charge = 0; // ���		
			rt_mb_send(mb_display, TO_fire_ok); // ֪ͨ��ʾˢ��
		}	
	}
}
/*******************************************************************************
* �� �� ��         : btm_servo_control
* ��������		   : �ײ�����ı�������
					������ʹ�ÿ�������
* ��    ��         : offset_x (���ź�-1~1)
* ��    ��         : ��
*******************************************************************************/

int8_t btm_servo_control(void)
{
	static float integral = 0;
	static float last_offset = 0;
	float delta_degree, next_btm_degree;
	 //������С��ֵ
    if(offset_x < offset_dead_block && offset_x > -offset_dead_block)
       offset_x = 0;
	//integral += offset_x;  // �õ�����
//	if(integral > 2)  // �����޷�
//		integral = 2;
//	else if(integral < -2)
//		integral = -2;
    // offset��Χ��-1��1���Ľ�����һ�������൱��ģ��p(��ƫ��С�Ĳ���)
//	if(offset_x < 0.033 && offset_x > -0.033)
//		delta_degree = offset_x * (btm_kp+15);
//	else if(offset_x < 0.25 && offset_x > -0.25)
//		delta_degree = offset_x * (btm_kp-5);
//	else 
//		delta_degree = offset_x * btm_kp;
	
	delta_degree = (offset_x-last_offset) * btm_kp + last_offset * btm_ki; // �Ľ�������������Ki
    // ����õ��µĵײ�����Ƕ�
    next_btm_degree = last_btm_degree + delta_degree;
    // ��ӱ߽���
    if(next_btm_degree < -30)
        next_btm_degree = -30;
    else if (next_btm_degree > 30)
        next_btm_degree = 30;
    last_btm_degree = next_btm_degree;
	last_offset = offset_x;
    return last_btm_degree;
}

/*******************************************************************************
* �� �� ��         : Elegun_autofire_thread_entry
* ��������		   : ���Ӳ��ֵ�һ����ں���
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void Elegun_autofire_thread_entry(void* parameter)  
{
	rt_err_t uwRet = RT_EOK;	
    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
		
		uwRet = rt_sem_take(sem_elegun_autofire, RT_WAITING_FOREVER); 	  /* �ȴ�ʱ�䣺һֱ */
		if(RT_EOK == uwRet)
		{
			rt_thread_mdelay(500); // �ӳ�һ��������ȶ�
			offset_x = ((float)receive_x/Img_width - 0.5) * 2;
			while(offset_x != 0) // ������̨���2
			{				
				ele_angle = btm_servo_control(); // ��������
				if(ele_angle < 0)
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (ele_angle) + 0.5 ) ;   // ��ת
				else
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (ele_angle) + 0.5 ) ;   // ��ת
				rt_thread_mdelay(pi_sample_time);  // �ö������λ
				if(offset_x)
					offset_x = ((float)receive_x/Img_width - 0.5) * 2;
			}
			USART_Cmd(camera_uart_device.uart_module, DISABLE);  // ��ʹ������ͷ�˹�uart����
			rt_thread_mdelay(1500); //�ӳ�1.5s�ó������ȶ������ٲ��
			ele_distance = (uint16_t)(Hcsr04GetLength()+Hcsr04GetLength())*0.5; // ��ȡ10��
			ele_distance = ele_distance - 30;  // �����Ͱ���λ����30cm
			rt_mb_send(mb_display, Wave_update); // ֪ͨ��ʾԭʼ�ó��ľ���
			if(ele_distance >= 300) // �����޷�
				ele_distance = 300;
			else if(ele_distance <= 200 )
				ele_distance = 200;
			////////////////����ת///////////////////////
			if(using_big_angle == 1)
			{
				dis_angle = asin(2*ele_distance/(dis_rate_big))/2 * 180.0/3.1416;  // �����Ǽ���
				pwm_set_Duty(&steer1, Steer1_S3010_mid + 
				(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * (90-dis_angle) + 0.5 );   // ����ת�Ƕȣ� ��ת
			}
			else if(using_big_angle == 0)
			{
				float fuzzy_rate = -0.0024*ele_distance*ele_distance + 0.2490*ele_distance + dis_rate_small;  //��Ͻ��
				dis_angle = asin(2*ele_distance/(fuzzy_rate))/2 * 180.0/3.1416;  // �����Ǽ���
				pwm_set_Duty(&steer1, Steer1_S3010_mid + 
				(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * (dis_angle) + 0.5 );   // ����ת�Ƕȣ� ��ת
			}
			/***���䲿��***/
			rt_thread_mdelay(2200); // ��ʱ���䣨���ڹ�������ȶ�������
			Fire = 1;// ���䵯��
			Charge = 1; // �رճ��
			rt_thread_mdelay(800); // ��ʱ800ms
			Fire = 0;
			Charge = 0; // �������
			rt_mb_send(mb_display, TO_fire_ok); // ֪ͨ��ʾˢ��
		}
	}
}

/*******************************************************************************
* �� �� ��         : Elegun_shakefire_thread_entry
* ��������		   : ���Ӳ��ֵڶ�����ں���
* ��    ��         : 
* ��    ��         : ��
*******************************************************************************/
void Elegun_shakefire_thread_entry(void* parameter)  
{
	rt_err_t uwRet = RT_EOK;	
    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
		
		uwRet = rt_sem_take(sem_elegun_shakefire, RT_WAITING_FOREVER); 	  /* �ȴ�ʱ�䣺һֱ */
		if(RT_EOK == uwRet)
		{
			float steer2_tempangle = -30;
			float fuzzy_rate = 0;
			ele_distance = 250; // ֱ�Ӹ���Ҫ��þ���Ϊ250cm
			////////////����ת�������ⲻ����ζ���С�Ƕ�/////////////////
			fuzzy_rate = -0.0024*ele_distance*ele_distance + 0.2490*ele_distance + dis_rate_small;  //��Ͻ��
			dis_angle = asin(2*ele_distance/(fuzzy_rate))/2 * 180.0/3.1416;  // �����Ǽ���
//			dis_angle = asin(2*ele_distance/dis_rate_small)/2 * 180.0/3.1416;  // �����Ǽ���
			pwm_set_Duty(&steer1, Steer1_S3010_mid + 
			(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * dis_angle + 0.5 );   // ��ת������
			
			elegun_shakefire_rotation = 1; // ��ǰ����ҡ
			while(steer2_tempangle <=30)
			{
				if(steer2_tempangle < 0)
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (steer2_tempangle) + 0.5 ) ;   // ��ת
				else
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (steer2_tempangle) + 0.5 ) ;   // ��ת
				rt_thread_mdelay(50); // ת����һ��
				steer2_tempangle++;
			}
			elegun_shakefire_rotation = 2; // ��ǰ����ҡ
			while(steer2_tempangle >= -30)
			{
				if(steer2_tempangle < 0)
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (steer2_tempangle) + 0.5 ) ;   // ��ת
				else
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (steer2_tempangle) + 0.5 ) ;   // ��ת
				rt_thread_mdelay(50); // ת����һ��
				steer2_tempangle--;
			}
			elegun_shakefire_rotation = 0; // ֮��ûҡ
		}
	}
}
	

