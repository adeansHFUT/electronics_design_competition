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
uint16_t ele_distance = 200; //�趨���� 
int8_t  ele_angle = 0;  // �趨�Ƕ�
uint8_t receive_x = 0; // ���ܵ���Xֵ
double dis_rate = 923;  // ����ת�Ƕȱ���(��С600����Ȼ�޷����㷴����) 923 FOR С��
float dis_angle = 0; // ���һת���ĽǶ�
float offset_x = 0;
float offset_dead_block = 0.02; // ƫ����������С(0.02���)
float  btm_kp = 15;  // ���ƶ����ת�ı���ϵ��(��ʼ15���)
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
			dis_angle = asin(2*ele_distance/dis_rate)/2 * 180.0/3.1416;  // �����Ǽ���
			pwm_set_Duty(&steer1, Steer1_S3010_mid + 
			(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * dis_angle );   // ����ת�Ƕȣ� ��ת
			if(ele_angle < 0)
				pwm_set_Duty(&steer2, Steer2_S3010_mid -
				(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (ele_angle) ) ;   // ��ת
			else
				pwm_set_Duty(&steer2, Steer2_S3010_mid -
				(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (ele_angle) ) ;   // ��ת
			/***���䲿��***/
			rt_thread_mdelay(1200); // ��ʱ1.2S����			
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
			rt_thread_mdelay(500); // �ӳ�һ��������ȶ�
			offset_x = ((float)receive_x/Img_width - 0.5) * 2;
			while(offset_x != 0) // ������̨���2
			{
				offset_x = ((float)receive_x/Img_width - 0.5) * 2;
				ele_angle = btm_servo_control(); // ��������
				if(ele_angle < 0)
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_max-Steer2_S3010_mid)/90.0) * (ele_angle) ) ;   // ��ת
				else
					pwm_set_Duty(&steer2, Steer2_S3010_mid -
					(float)((Steer2_S3010_mid - Steer2_S3010_min)/90.0) * (ele_angle) ) ;   // ��ת
				rt_thread_mdelay(35);  // �ö������λ
			}
			USART_Cmd(camera_uart_device.uart_module, DISABLE);  // ��ʹ������ͷ�˹�uart����
			ele_distance = (uint16_t)(Hcsr04GetLength()+Hcsr04GetLength())*0.5; // ��ȡ10��
			ele_distance = ele_distance - 30;  // �����Ͱ���λ����30cm
			rt_mb_send(mb_display, Wave_update); // ֪ͨ��ʾԭʼ�ó��ľ���
			if(ele_distance >= 300) // �����޷�
				ele_distance = 300;
			else if(ele_distance <= 200 )
				ele_distance = 200;
			dis_angle = asin(2*ele_distance/dis_rate)/2 * 180.0/3.1416;  // �����Ǽ���
			pwm_set_Duty(&steer1, Steer1_S3010_mid + 
			(float)((Steer1_S3010_max-Steer1_S3010_mid)/90.0) * dis_angle );   // ����ת�Ƕȣ� ��ת
			/***���䲿��***/
			rt_thread_mdelay(1200); // ��ʱ����
			Fire = 1;// ���䵯��
			Charge = 1; // �رճ��
			rt_thread_mdelay(800); // ��ʱ800ms
			Fire = 0;
			Charge = 0; // �������
			rt_mb_send(mb_display, TO_fire_ok); // ֪ͨ��ʾˢ��
		}
	}
}