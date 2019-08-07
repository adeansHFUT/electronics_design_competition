#include "Elegun_handle.h"
#include "bsp_pwm.h"
#include "bsp_steer.h"
#include <stdlib.h>
rt_thread_t Elegun_thread = RT_NULL;
rt_thread_t Elegun_autofire_thread = RT_NULL;
rt_sem_t sem_elegun = RT_NULL;
rt_sem_t sem_elegun_autofire = RT_NULL;
uint16_t ele_distance = 200; //�趨���� 
int8_t  ele_angle = 0;  // �趨�Ƕ�
uint8_t receive_x = 0; // ���ܵ���Xֵ

float offset_x = 0;
float offset_dead_block = 0.1; // ƫ����������С
float  btm_kp = 5;  // ���ƶ����ת�ı���ϵ��
int8_t  last_btm_degree = 0; // ��һ�εײ�����ĽǶ�
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
			//���ת��ָ��λ��
			pwm_set_Duty(&steer1, Steer1_S3010_mid + 
			(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * ele_distance*0.1 );   // ����ת�Ƕȣ� ��ת
			if(ele_angle < 0)
				pwm_set_Duty(&steer2, Steer2_S3010_mid -
				(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (ele_angle) ) ;   // ��ת
			else
				pwm_set_Duty(&steer2, Steer2_S3010_mid -
				(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (ele_angle) ) ;   // ��ת
			//���䵯��
			
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
	int8_t delta_degree, next_btm_degree;
	 //������С��ֵ
    if(offset_x < offset_dead_block && offset_x > -offset_dead_block)
       offset_x = 0;
 
    // offset��Χ��-50��50����
    delta_degree = offset_x * btm_kp;
    // ����õ��µĵײ�����Ƕ�
    next_btm_degree = last_btm_degree + delta_degree;
    // ��ӱ߽���
    if(next_btm_degree < -30)
        next_btm_degree = -30;
    else if (next_btm_degree > 30)
        next_btm_degree = 30;
    last_btm_degree = next_btm_degree;
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
			rt_thread_mdelay(1000); // �ӳ�һ��������ȶ�
			offset_x = ((float)receive_x/Img_width - 0.5) * 2;
			while(offset_x != 0)
			{
				offset_x = ((float)receive_x/Img_width - 0.5) * 2;
				ele_angle = btm_servo_control(); // ��������
				if(ele_angle < 0)
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (ele_angle) ) ;   // ��ת
				else
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (ele_angle) ) ;   // ��ת
			}		
		}
	}
}