#include "key_handle.h"
#include "rtthread.h"
#include "board.h"
#include "include.h"

/* ���尴�������߳̿��ƿ� */
rt_thread_t keyhandle_thread = RT_NULL;

/*******************************�������ڵ�״̬(��ʼΪMainmeau̬)************************************************/
Statename current_state = Mainmeau; 
typedef struct {
	Statename state_name;
	Action action;
}state;
/*******************************����״̬ת�Ʊ�************************************************/
state state_transition[10][10]; 
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
	state_transition[Mainmeau][KEY_1].state_name = Testmeau;
	state_transition[Mainmeau][KEY_1].action = Mainmeau_to_Testmeau;
	state_transition[Mainmeau][KEY_2].state_name = Task_randw;
	state_transition[Mainmeau][KEY_2].action = Mainmeau_to_Task_randw;
	state_transition[Mainmeau][KEY_3].state_name = Task2;
	state_transition[Mainmeau][KEY_3].action = Mainmeau_to_Task2;
	state_transition[Mainmeau][KEY_4].state_name = Mainmeau;
	state_transition[Mainmeau][KEY_4].action = NOaction;
	
	state_transition[Testmeau][KEY_1].state_name = Testmeau;
	state_transition[Testmeau][KEY_1].action = NOaction;
	state_transition[Testmeau][KEY_2].state_name = Testmeau;
	state_transition[Testmeau][KEY_2].action = NOaction;
	state_transition[Testmeau][KEY_3].state_name = Testmeau;
	state_transition[Testmeau][KEY_3].action = NOaction;
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
	
	state_transition[Task2][KEY_1].state_name = Task2;
	state_transition[Task2][KEY_1].action = NOaction;
	state_transition[Task2][KEY_2].state_name = Task2;
	state_transition[Task2][KEY_2].action = NOaction;
	state_transition[Task2][KEY_3].state_name = Task2;
	state_transition[Task2][KEY_3].action = NOaction;
	state_transition[Task2][KEY_4].state_name = Mainmeau;
	state_transition[Task2][KEY_4].action = Task2_to_Mainmeau;
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
* ��    ע 		   ��
*******************************************************************************/
void state_transfer(uint8_t statenow, uint8_t key_receive)
{
	switch(state_transition[statenow][key_receive].action){
		case NOaction:{
               // ״̬���˵��޶���,һ�㲻������
			break;
		}
		case Mainmeau_to_Testmeau:{
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			break;
		}
		case Testmeau_to_Mainmeau:{
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			break;
		}
		case Mainmeau_to_Task_randw:{
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			rt_mb_send(mb_ctrlAt24, state_transition[statenow][key_receive].action);/*�����֪ͨ��������Ĳ���*/
			break;
		}
		case Task_randw_to_Mainmeau:{
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			/*�����֪ͨ��������Ĳ���*/
			break;
		}
		case Mainmeau_to_Task2:{
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			/*�����֪ͨ��������Ĳ���*/
			break;
		}
		case Task2_to_Mainmeau: {
			rt_mb_send(mb_display, state_transition[statenow][key_receive].action);
			/*�����֪ͨ��������Ĳ���*/
			break;
		}
		default:{
			rt_kprintf("״̬ת�ƴ���,action:%d\n", state_transition[statenow][key_receive].action);
			break;
		}
					
	}
	key_num = 0;		
	rt_enter_critical(); // ֱ�ӹص��������ú�
	current_state = state_transition[statenow][key_receive].state_name; //����״̬
	rt_exit_critical();
	return;
	
		
}