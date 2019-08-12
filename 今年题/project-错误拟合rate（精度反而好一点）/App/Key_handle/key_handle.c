#include "key_handle.h"
#include "rtthread.h"
#include "board.h"
#include "include.h"

/* ���尴�������߳̿��ƿ� */
rt_thread_t keyhandle_thread = RT_NULL;
uint16_t test_duty = Steer1_S3010_mid;
/*******************************�������ڵ�״̬(��ʼΪMainmeau̬)************************************************/
Statename current_state = Mainmeau;  
typedef struct {
	Statename state_name;
	Action action;
}state;
/*******************************����״̬ת�Ʊ�************************************************/
state state_transition[20][10] = {0}; 
/*******************************��������************************************************/
void state_transfer(uint8_t statenow, uint8_t key_receive);
/*******************************************************************************
* �� �� ��         : statetable_init
* ��������		   : ����������״̬ת�Ʊ��ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void statetable_init(void)
{

	state_transition[Mainmeau][KEY_1].action = Mainmeau_to_Testmeau;
	state_transition[Mainmeau][KEY_1].state_name= Testmeau;
	state_transition[Mainmeau][KEY_2].action = Mainmeau_to_Task_randw;
	state_transition[Mainmeau][KEY_2].state_name = Task_randw;
	state_transition[Mainmeau][KEY_3].state_name = Pos_input; // ��������
	state_transition[Mainmeau][KEY_3].action = Mainmeau_to_Pos_input;
	state_transition[Mainmeau][KEY_4].state_name = Steer_test;  //�������
	state_transition[Mainmeau][KEY_4].action = Mainmeau_to_Steer_test;
	state_transition[Mainmeau][KEY_5].state_name = Elegun_autofire_set; // ����һ
	state_transition[Mainmeau][KEY_5].action = Mainmeau_to_Elegun_autofire_set;
	state_transition[Mainmeau][KEY_6].state_name = Elegun_shakefire_set; // ���Ӷ�
	state_transition[Mainmeau][KEY_6].action = Mainmeau_to_Elegun_shakefire_set;
	state_transition[Mainmeau][KEY_7].action = UsingAT24_switch;  // �л��Ƿ�ʹ��AT24Ĭ��ʹ��
	state_transition[Mainmeau][KEY_8].action = Using_big_angle_switch;  // ʹ�ô�Ƕ��л�
	
	
	state_transition[Testmeau][KEY_4].state_name = Mainmeau;
	state_transition[Testmeau][KEY_4].action = Testmeau_to_Mainmeau;
	
	state_transition[Task_randw][KEY_1].state_name = Task_randw;
	state_transition[Task_randw][KEY_1].action = Pidplus;
	state_transition[Task_randw][KEY_2].state_name = Task_randw;
	state_transition[Task_randw][KEY_2].action = Pidminus;
	state_transition[Task_randw][KEY_3].state_name = Task_randw;
	state_transition[Task_randw][KEY_3].action = Pidwrite;
	state_transition[Task_randw][KEY_4].state_name = Mainmeau;
	state_transition[Task_randw][KEY_4].action = Task_randw_to_Mainmeau;
		
	state_transition[Banqiu_setA][KEY_6].state_name = Banqiu_setB;
	state_transition[Banqiu_setA][KEY_6].action = Banqiu_setA_to_Banqiu_setB;
	state_transition[Banqiu_setA][KEY_1].action = Banqiu_setA_plus;
	state_transition[Banqiu_setA][KEY_2].action = Banqiu_setA_minus;
	state_transition[Banqiu_setA][KEY_3].state_name= Banqiu_set_pid;
	state_transition[Banqiu_setA][KEY_3].action = Banqiu_setA_to_Banqiu_set_pid;
	state_transition[Banqiu_setA][KEY_4].state_name = Mainmeau;
	state_transition[Banqiu_setA][KEY_4].action = Banqiu_setA_to_Mainmeau;
	
	state_transition[Banqiu_setB][KEY_1].action = Banqiu_setB_plus;
	state_transition[Banqiu_setB][KEY_2].action = Banqiu_setB_minus;
	state_transition[Banqiu_setB][KEY_3].state_name= Banqiu_set_pid;
	state_transition[Banqiu_setB][KEY_3].action = Banqiu_setB_to_Banqiu_set_pid;
	state_transition[Banqiu_setB][KEY_4].state_name = Mainmeau;
	state_transition[Banqiu_setB][KEY_4].action = Banqiu_setB_to_Mainmeau;
	
	state_transition[Banqiu_set_pid][KEY_5].action = Banqiu_P_plus;
	state_transition[Banqiu_set_pid][KEY_6].action = Banqiu_P_minus;
	state_transition[Banqiu_set_pid][KEY_7].action = Banqiu_I_plus;
	state_transition[Banqiu_set_pid][KEY_8].action = Banqiu_I_minus;
	state_transition[Banqiu_set_pid][KEY_1].action = Banqiu_D_plus;
	state_transition[Banqiu_set_pid][KEY_2].action = Banqiu_D_minus;
	state_transition[Banqiu_set_pid][KEY_3].action = Banqiu_next;
	state_transition[Banqiu_set_pid][KEY_4].state_name = Mainmeau;
	state_transition[Banqiu_set_pid][KEY_4].action = Banqiu_set_pid_to_Mainmeau;
	
	state_transition[Steer_test][KEY_1].action = Steer_plus;
	state_transition[Steer_test][KEY_2].action = Steer_minus;
	state_transition[Steer_test][KEY_4].state_name = Mainmeau;
	state_transition[Steer_test][KEY_4].action = Steer_test_to_Mainmeau;
	
	state_transition[Pos_input][KEY_1].action = Distance_rate_plus;
	state_transition[Pos_input][KEY_2].action = Distance_rate_minus;
	state_transition[Pos_input][KEY_5].action = Distance_plus;
	state_transition[Pos_input][KEY_6].action = Distance_minus;
	state_transition[Pos_input][KEY_7].action = Angle_plus;
	state_transition[Pos_input][KEY_8].action = Angle_minus;
	state_transition[Pos_input][KEY_3].state_name = Steer_move_fire;
	state_transition[Pos_input][KEY_3].action = Pos_input_to_Steer_move_fire;
	state_transition[Pos_input][KEY_4].state_name = Mainmeau;
	state_transition[Pos_input][KEY_4].action = Pos_input_to_Mainmeau;
	
	state_transition[Steer_move_fire][KEY_4].state_name = Pos_input;
	state_transition[Steer_move_fire][KEY_4].action = Steer_move_fire_to_Pos_input;
	
	state_transition[Elegun_autofire_set][KEY_4].state_name = Mainmeau;
	state_transition[Elegun_autofire_set][KEY_4].action = Elegun_autofire_set_to_Mainmeau;
	state_transition[Elegun_autofire_set][KEY_1].action = Autofire_Kp_plus;
	state_transition[Elegun_autofire_set][KEY_2].action = Autofire_Kp_minus;
	state_transition[Elegun_autofire_set][KEY_5].action = Autofire_Ki_plus;
	state_transition[Elegun_autofire_set][KEY_6].action = Autofire_Ki_minus;
	state_transition[Elegun_autofire_set][KEY_7].action = Dead_block_plus;
	state_transition[Elegun_autofire_set][KEY_8].action = Dead_block_minus;
	state_transition[Elegun_autofire_set][KEY_3].state_name = Elegun_autofire;
	state_transition[Elegun_autofire_set][KEY_3].action = Elegun_autofire_set_to_Elegun_autofire;
	
	state_transition[Elegun_shakefire_set][KEY_1].action = Advance_amount_plus;
	state_transition[Elegun_shakefire_set][KEY_2].action = Advance_amount_minus;
	state_transition[Elegun_shakefire_set][KEY_4].state_name = Mainmeau;
	state_transition[Elegun_shakefire_set][KEY_4].action = Elegun_shakefire_set_to_Mainmeau;
	state_transition[Elegun_shakefire_set][KEY_3].state_name = Elegun_shakefire;
	state_transition[Elegun_shakefire_set][KEY_3].action = Elegun_shakefire_set_to_Elegun_shakefire;
	state_transition[Elegun_shakefire_set][KEY_5].action = Pi_sample_plus;
	state_transition[Elegun_shakefire_set][KEY_6].action = Pi_sample_minus;

	
	state_transition[Elegun_autofire][KEY_4].state_name = Elegun_autofire_set;
	state_transition[Elegun_autofire][KEY_4].action = Elegun_autofire_to_Elegun_autofire_set;
	
	state_transition[Elegun_shakefire][KEY_4].state_name = Elegun_shakefire_set;
	state_transition[Elegun_shakefire][KEY_4].action = Elegun_shakefire_to_Elegun_shakefire_set;
}
/*******************************************************************************
* �� �� ��         : keyhandle_thread_entry
* ��������		   : ���������߳���ں���
* ��    ��         : ��
* ��    ��         : ��
* ��    ע 		   �����ȼ�Ҫ���ڸ������񣬱�֤��������ʵʱ�ԣ������û�����
*******************************************************************************/
void keyhandle_thread_entry(void* parameter)
{
	rt_err_t uwRet = RT_EOK;	
	rt_uint32_t keynum;
    /* ������һ������ѭ�������ܷ��� */
    while (1)
    {
		uwRet = rt_mb_recv(mb_key, &keynum, RT_WAITING_FOREVER); 	  /* �ȴ�ʱ�䣺һֱ */
		if(RT_EOK == uwRet)
		{
		    rt_kprintf("thread_keyhandle���յ�����:%d,��ʼ����\n",keynum);
			state_transfer(current_state, keynum);
		}
		else
			rt_kprintf("thread_keyhandle����������ʧ�ܣ�\n");
    }
}
/*******************************************************************************
* �� �� ��         : state_transfer
* ��������		   : ״̬ת�ƺ���
* ��    ��         : ��
* ��    ��         : ��
* ��    ע 		   ������ϵͳ�İ����¼�ʶ����ת��������
*******************************************************************************/
void state_transfer(uint8_t statenow, uint8_t key_receive)
{
	switch(state_transition[statenow][key_receive].action){
		case NOaction:{
               // ״̬���˵��޶���,һ�㲻������
			break;
		}
/****************Mainmeau״̬����*********************/
		case Mainmeau_to_Testmeau:{
			rt_mb_send(mb_display, Mainmeau_to_Testmeau);
			break;
		}
		case Mainmeau_to_Task_randw:{
			rt_mb_send(mb_ctrlAt24, Mainmeau_to_Task_randw);/*֪ͨAT24��pid*/
			rt_mb_send(mb_display, Mainmeau_to_Task_randw);
			break;
		}
		case Mainmeau_to_Banqiu_setA:{
			rt_mb_send(mb_display, Mainmeau_to_Banqiu_setA);
			break;
		}
		case Mainmeau_to_Steer_test:{
			rt_mb_send(mb_display, Mainmeau_to_Steer_test);
			break;
		}
		case Mainmeau_to_Pos_input:{
			rt_mb_send(mb_display, Mainmeau_to_Pos_input);
			rt_mb_send(mb_ctrlAt24, Mainmeau_to_Pos_input);
			break;
		}
		case Mainmeau_to_Elegun_autofire_set:{
			rt_mb_send(mb_display, Mainmeau_to_Elegun_autofire_set);
			rt_mb_send(mb_ctrlAt24, Mainmeau_to_Elegun_autofire_set);
			break;
		}
		case Mainmeau_to_Elegun_shakefire_set:{
			rt_mb_send(mb_display, Mainmeau_to_Elegun_shakefire_set);
			rt_mb_send(mb_ctrlAt24, Mainmeau_to_Elegun_shakefire_set);
			break;
		}
		case UsingAT24_switch:{ 									// �Է�AT24���ˣ����������ڶ�AT24��
			using_at24 = !using_at24;
			rt_mb_send(mb_display, UsingAT24_switch);
			break;
		}
		case AT24_cover:{  											// �Է�AT24��Ĳ�����ʧ��Ϊ�������
			AT24CXX_WriteOneByte(10, (uint16_t)(dis_rate_big)/100);
			AT24CXX_WriteOneByte(11, (uint16_t)(dis_rate_big)%100);
			AT24CXX_WriteOneByte(12, (uint8_t)btm_kp);
			AT24CXX_WriteOneByte(13, (uint8_t)btm_ki);
			AT24CXX_WriteOneByte(14, (uint8_t)(offset_dead_block*1000));
			AT24CXX_WriteOneByte(15, (uint8_t)shake_advance_amount);
			AT24CXX_WriteOneByte(16, (uint8_t)pi_sample_time);
			break;
		}
		case Using_big_angle_switch:{
			using_big_angle = !using_big_angle;
			rt_mb_send(mb_display, Using_big_angle_switch);
			break;
		}
/****************Testmeau״̬����*********************/
		case Testmeau_to_Mainmeau:{
			rt_mb_send(mb_display, Testmeau_to_Mainmeau);
			break;
		}
/****************Task_randw״̬����*********************/		
		case Task_randw_to_Mainmeau:{
			rt_mb_send(mb_display, Task_randw_to_Mainmeau);
			break;
		}
		case Pidplus:case Pidminus:case Pidwrite:{
			rt_mb_send(mb_ctrlAt24, state_transition[statenow][key_receive].action);
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			break;
		}
/****************Banqiu_setX״̬����*********************/	
//		case Banqiu_setA_to_Banqiu_set_pid:case Banqiu_setB_to_Banqiu_set_pid:{
//			USART_Cmd(camera_uart_device.uart_module, ENABLE);  // ��uart����
//			rt_sem_release(sem_Banqiu_task);  // ��������task1�߳�
//			rt_mb_send(mb_ctrlAt24, state_transition[statenow][key_receive].action); /*֪ͨAT24��pid*/
//			rt_mb_send(mb_display, state_transition[statenow][key_receive].action); // ֪ͨ��ʾˢ��	
//			break;
//		}
//		case Banqiu_setA_to_Banqiu_setB:case Banqiu_setA_to_Mainmeau:case Banqiu_setB_to_Mainmeau:{
//			rt_mb_send(mb_display, state_transition[statenow][key_receive].action); // ֪ͨ��ʾˢ��	
//			break;
//		}
//		case Banqiu_setA_plus:{
//			targetA++;
//			target_point[0] = standing_point[targetA];
//			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
//			break;
//		}
//		case Banqiu_setA_minus:{
//			targetA--;
//			target_point[0] = standing_point[targetA];
//			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
//			break;
//		}
//		case Banqiu_setB_plus:{
//			targetB++;
//			target_point[1] = standing_point[targetB];
//			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
//			break;
//		}
//		case Banqiu_setB_minus:{
//			targetB--;
//			target_point[1] = standing_point[targetB];
//			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
//			break;
//		}
/****************Banqiu_set_pid״̬����*********************/		
//		case Banqiu_set_pid_to_Mainmeau: {
//			USART_Cmd(camera_uart_device.uart_module, DISABLE);  // �ر�uart����
//			rt_sem_take(sem_Banqiu_task, RT_WAITING_FOREVER); // ����������1��ͣ����(����)
//			rt_mb_send(mb_ctrlAt24, Banqiu_set_pid_to_Mainmeau);  // �˳�����pid
//			rt_mb_send(mb_display, Banqiu_set_pid_to_Mainmeau);
//			break;
//		}
//		case Banqiu_P_plus:{
//			pos_pid_control_set_kp(pid_steer1, pid_steer1->kp + 0.1);
//			pos_pid_control_set_kp(pid_steer2, pid_steer2->kp + 0.1);
//			rt_mb_send(mb_display, Banqiu_P_plus);
//			break;
//		}
//		case Banqiu_P_minus:{
//			pos_pid_control_set_kp(pid_steer1, pid_steer1->kp - 0.1);
//			pos_pid_control_set_kp(pid_steer2, pid_steer2->kp - 0.1);
//			rt_mb_send(mb_display, Banqiu_P_minus);
//			break;
//		}
//		case Banqiu_I_plus:{
//			pos_pid_control_set_ki(pid_steer1, pid_steer1->ki + 0.1);
//			pos_pid_control_set_ki(pid_steer2, pid_steer2->ki + 0.1);
//			rt_mb_send(mb_display, Banqiu_I_plus);
//			break;
//		}
//		case Banqiu_I_minus:{
//			pos_pid_control_set_ki(pid_steer1, pid_steer1->ki - 0.1);
//			pos_pid_control_set_ki(pid_steer2, pid_steer2->ki - 0.1);
//			rt_mb_send(mb_display, Banqiu_I_minus);
//			break;
//		}
//		case Banqiu_D_plus:{
//			pos_pid_control_set_kd(pid_steer1, pid_steer1->kd + 0.1);
//			pos_pid_control_set_kd(pid_steer2, pid_steer2->kd + 0.1);
//			rt_mb_send(mb_display, Banqiu_D_plus);
//			break;
//		}
//		case Banqiu_D_minus:{
//			pos_pid_control_set_kd(pid_steer1, pid_steer1->kd - 0.1);
//			pos_pid_control_set_kd(pid_steer2, pid_steer2->kd - 0.1);
//			rt_mb_send(mb_display, Banqiu_D_minus);
//			break;
//		}
//		case Banqiu_next:{
//			if(target_point[target_now+1].number!= 0)
//				target_now++;
//			rt_mb_send(mb_display, Banqiu_next);	
//			break;			
//		}
/****************steer_test*********************/	
		case Steer_test_to_Mainmeau:{
			rt_mb_send(mb_display, Steer_test_to_Mainmeau);
			break;
		}
		case Steer_plus:{
			test_duty++;
			pwm_set_Duty(&steer1, test_duty);
			rt_mb_send(mb_display, Steer_plus);
			break;
		}
		case Steer_minus:{
			test_duty--;
			pwm_set_Duty(&steer1, test_duty);
			rt_mb_send(mb_display, Steer_minus);
			break;
		}
/****************Pos_input״̬����*********************/	
		case Pos_input_to_Mainmeau:{
			rt_mb_send(mb_display, Pos_input_to_Mainmeau);
			rt_mb_send(mb_ctrlAt24, Pos_input_to_Mainmeau);
			break;
		}
		case Pos_input_to_Steer_move_fire:{
			rt_sem_release(sem_elegun);   // ֪ͨ����ڷ���
			rt_mb_send(mb_display, Pos_input_to_Steer_move_fire);
			break;
		}
		case Distance_rate_plus:{   
			if(using_big_angle == 1) // 
				dis_rate_big++;
			else if(using_big_angle == 0)
				dis_rate_small++;
			rt_mb_send(mb_display, Distance_rate_plus);
			break;
		}
		case Distance_rate_minus:{
			if(using_big_angle == 1)
				dis_rate_big--;
			else if(using_big_angle == 0)
				dis_rate_small--;
			rt_mb_send(mb_display, Distance_rate_minus);
			break;
		}
		case Distance_plus:{
			ele_distance++;
			rt_mb_send(mb_display, Distance_plus);
			break;
		}
		case Distance_plus_ten:{
			ele_distance+=10;
			rt_mb_send(mb_display, Distance_plus_ten);
			break;
		}
		case Distance_minus:{
			ele_distance--;
			rt_mb_send(mb_display, Distance_minus);
			break;
		}
		case Angle_plus:{
			ele_angle++;
			rt_mb_send(mb_display, Angle_plus);
			break;
		}
		case Angle_minus:{
			ele_angle--;
			rt_mb_send(mb_display, Angle_minus);
			break;
		}
/****************Steer_move_fire״̬����*********************/	
		case Steer_move_fire_to_Pos_input:{
			pwm_set_Duty(&steer1, Steer1_S3010_mid); // �����������
			pwm_set_Duty(&steer2, Steer2_S3010_mid);
			rt_mb_send(mb_display, Steer_move_fire_to_Pos_input);
			break;
		}
		
/****************Elegun_autofire_set״̬����*********************/	
		case Elegun_autofire_set_to_Elegun_autofire:{
			USART_Cmd(camera_uart_device.uart_module, ENABLE);  // ������ͷuart����
			rt_sem_release(sem_elegun_autofire);
			rt_mb_send(mb_display, Elegun_autofire_set_to_Elegun_autofire);
			break;
		}
		case Elegun_autofire_set_to_Mainmeau:{
			rt_mb_send(mb_display, Elegun_autofire_set_to_Mainmeau);
			rt_mb_send(mb_ctrlAt24, Elegun_autofire_set_to_Mainmeau);
			break;
		}
		case Autofire_Kp_plus:{
			btm_kp++;
			rt_mb_send(mb_display, Autofire_Kp_plus);
			break;
		}
		case Autofire_Kp_minus:{
			btm_kp--;
			rt_mb_send(mb_display, Autofire_Kp_minus);
			break;
		}
		case Autofire_Ki_plus:{
			btm_ki++;
			rt_mb_send(mb_display, Autofire_Ki_plus);
			break;
		}
		case Autofire_Ki_minus:{
			btm_ki--;
			rt_mb_send(mb_display, Autofire_Ki_minus);
			break;
		}
		case Dead_block_plus:{
			offset_dead_block += 0.001; // ƫ����������С(0.02���)
			rt_mb_send(mb_display, Dead_block_plus);
			break;
		}
		case Dead_block_minus:{
			offset_dead_block -= 0.001; // ƫ����������С(0.02���)
			rt_mb_send(mb_display, Dead_block_minus);
			break;
		}
/****************Elegun_autofire״̬����*********************/	
		case Elegun_autofire_to_Elegun_autofire_set:{
			USART_Cmd(camera_uart_device.uart_module, DISABLE);  // ��uart����
			pwm_set_Duty(&steer1, Steer1_S3010_mid); // �����������
			pwm_set_Duty(&steer2, Steer2_S3010_mid);
			last_btm_degree = 0;  // �������ϴ�����Ƕ�����
			rt_mb_send(mb_display, Elegun_autofire_to_Elegun_autofire_set);
			break;
		}
		
/****************Elegun_shakefire_set״̬����*********************/	
		case Elegun_shakefire_set_to_Elegun_shakefire:{
			USART_Cmd(camera_uart_device.uart_module, ENABLE);  // ������ͷuart����
			rt_sem_release(sem_elegun_shakefire);
			rt_mb_send(mb_display, Elegun_shakefire_set_to_Elegun_shakefire);
			break;
		}
		case Elegun_shakefire_set_to_Mainmeau:{
			rt_mb_send(mb_display, Elegun_shakefire_set_to_Mainmeau);
			rt_mb_send(mb_ctrlAt24, Elegun_shakefire_set_to_Mainmeau);
			break;
		}
		case Advance_amount_plus:{
			shake_advance_amount++;
			rt_mb_send(mb_display, Advance_amount_plus);
			break;	
		}
		case Advance_amount_minus:{
			shake_advance_amount--;
			rt_mb_send(mb_display, Advance_amount_minus);
			break;	
		}
		case Pi_sample_plus:{
			pi_sample_time++;
			rt_mb_send(mb_display, Pi_sample_plus);
			break;
		}
		case Pi_sample_minus:{
			pi_sample_time--;
			rt_mb_send(mb_display, Pi_sample_minus);
			break;
		}
/****************Elegun_shakefire״̬����*********************/	
		case Elegun_shakefire_to_Elegun_shakefire_set:{
			USART_Cmd(camera_uart_device.uart_module, DISABLE);  // ��uart����
			pwm_set_Duty(&steer1, Steer1_S3010_mid); // �����������
			pwm_set_Duty(&steer2, Steer2_S3010_mid);
			last_btm_degree = 0;  // �������ϴ�����Ƕ�����
			rt_mb_send(mb_display, Elegun_shakefire_to_Elegun_shakefire_set);
			break;
		}
/****************default*********************/	
		default:{
			rt_kprintf("״̬ת�ƴ���,action:%d\n", state_transition[statenow][key_receive].action);
			break;
		}
					
	}
	if(state_transition[statenow][key_receive].state_name != SameState)	
	{
		rt_enter_critical(); // ֱ�ӹص��������ú�
		current_state = state_transition[statenow][key_receive].state_name; //����״̬
		rt_exit_critical();
	}		
	key_num = 0;  // ����֪ͨ��Ϻ�������¶�ȡ����
	return;
}