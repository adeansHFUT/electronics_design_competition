#include "bsp_sr04.h"
#include "PORT_cfg.h"
#include "sys.h"
#include "rtthread.h"
#include "board.h"

u16 msHcCount = 0;//ms����

void hcsr04_NVIC();

void Hcsr04Init()
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //�������ڶ�ʱ�����õĽṹ��
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(HCSR04_CLK, ENABLE);
     
        //IO��ʼ��
    GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;       //���͵�ƽ����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
     
    GPIO_InitStructure.GPIO_Pin =   HCSR04_ECHO;     //���ص�ƽ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
    GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);    
     
            //��ʱ����ʼ�� ʹ�û�����ʱ��TIM6
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);   //ʹ�ܶ�ӦRCCʱ��
        //���ö�ʱ�������ṹ��
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period = (1000-1); //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ         ������1000Ϊ1ms
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  1M�ļ���Ƶ�� 1US����
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ         
        
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);   //��������жϣ����һ���ж����������ж�
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);    //�򿪶�ʱ�������ж�
    hcsr04_NVIC();
	TIM_Cmd(TIM6,DISABLE);      // �ȹر���
}


//tips��static����������������ڶ�������Դ�ļ��ڣ����Բ���Ҫ��ͷ�ļ�������
static void OpenTimerForHc()        //�򿪶�ʱ��
{
        TIM_SetCounter(TIM6,0);//�������
        msHcCount = 0;
        TIM_Cmd(TIM6, ENABLE);  //ʹ��TIMx����
}
 
static void CloseTimerForHc()        //�رն�ʱ��
{
        TIM_Cmd(TIM6, DISABLE);  //ʹ��TIMx����
}
 
 
 //NVIC����
void hcsr04_NVIC()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;             //ѡ�񴮿�1�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռʽ�ж����ȼ�����Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //��Ӧʽ�ж����ȼ�����Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        //ʹ���ж�
    NVIC_Init(&NVIC_InitStructure);
}


//��ʱ��6�жϷ������
void TIM6_IRQHandler(void)   //TIM3�ж�
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //���TIM�����жϷ������
    {
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
        msHcCount++;   // ֮�������RT_gettick����
    }
}
 

//��ȡ��ʱ��ʱ��
u32 GetEchoTimer(void)
{
	u32 t = 0;
    t = msHcCount*1000;//�õ�MS
    t += TIM_GetCounter(TIM6);//�õ�US
    TIM6->CNT = 0;  //��TIM2�����Ĵ����ļ���ֵ����
    rt_thread_mdelay(50); // ������ʱ50ms,���β��֮����Ҫ���һ��ʱ��
    return t;
}
 

//һ�λ�ȡ������������� ���β��֮����Ҫ���һ��ʱ�䣬���ϻ����ź�
//Ϊ�����������Ӱ�죬ȡ������ݵ�ƽ��ֵ���м�Ȩ�˲���
float Hcsr04GetLength(void )
{
    u32 t = 0;
    int i = 0;
    float lengthTemp = 0;
    float sum = 0;
    while(i!=5)
    {
        TRIG_Send = 1;      //���Ϳڸߵ�ƽ���
        rt_hw_us_delay(20);
        TRIG_Send = 0;
        while(ECHO_Reci == 0);      //�ȴ����տڸߵ�ƽ���
        OpenTimerForHc();        //�򿪶�ʱ��
        i = i + 1;
        while(ECHO_Reci == 1);
        CloseTimerForHc();        //�رն�ʱ��
        t = GetEchoTimer();        //��ȡʱ��,�ֱ���Ϊ1US
        lengthTemp = ((float)t/58.0);//cm
        sum = lengthTemp + sum ;       
    }
    lengthTemp = sum/5.0;  // ��ε�����ƽ�� ��λcm
    return lengthTemp;
}


