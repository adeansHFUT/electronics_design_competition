#include "bsp_usart.h"
#include "rtthread.h"

/* �ⲿ�����ź������ƿ� */
extern rt_sem_t sem_debug_uart;
extern rt_sem_t sem_camera_uart;
char debug_Usart_Rx_Buf[DEBUG_USART_RBUFF_SIZE];  // ����洢�ռ�
uint8_t camera_Usart_Rx_Buf[Camera_USART_RBUFF_SIZE];  // ����ռ�Ҫ�����涨�壬ֱ���ڽṹ���ж���������飬dma�ʹ�����ȥ����֪Ϊɶ
My_uart_device debug_uart_device, camera_uart_device;  // ȫ�ֱ���
void my_uart_DMA_Rx_Data(My_uart_device uart_device, rt_sem_t sem_uart);
void my_uart_DMA_Config(My_uart_device uart_device);
void my_uart_Config(My_uart_device uart_device);

 /**
  * @brief  �������д����豸
  * @param  ��
  * @retval ��
  */
void uart_device_init(void)
{
	debug_uart_device.uart_module = DEBUG_USARTx;
	debug_uart_device.uart_rx_port = DEBUG_USART_RX_GPIO_PORT;
	debug_uart_device.uart_rx_pin = DEBUG_USART_RX_GPIO_PIN;
	debug_uart_device.uart_tx_port = DEBUG_USART_TX_GPIO_PORT;
	debug_uart_device.uart_tx_pin = DEBUG_USART_TX_GPIO_PIN;
	debug_uart_device.uart_gpio_RCC = DEBUG_USART_GPIO_CLK;
	debug_uart_device.uart_RCC = DEBUG_USART_CLK;
	debug_uart_device.uart_irq = DEBUG_USART_IRQ;
	debug_uart_device.baudrate = DEBUG_USART_BAUDRATE;
	debug_uart_device.dma_address = DEBUG_USART_DR_ADDRESS;
	debug_uart_device.dma_channel = DEBUG_USART_RX_DMA_CHANNEL;
	debug_uart_device.usart_Rx_Buf = debug_Usart_Rx_Buf;
	debug_uart_device.buff_size = DEBUG_USART_RBUFF_SIZE;
	debug_uart_device.uart_PrePriority = 1; // ��ռ���ȼ�
	debug_uart_device.uart_subPriority = 1;
	
	camera_uart_device.uart_module = Camera_USARTx;
	camera_uart_device.uart_rx_port = Camera_USART_RX_GPIO_PORT;
	camera_uart_device.uart_rx_pin = Camera_USART_RX_GPIO_PIN;
	camera_uart_device.uart_tx_port = Camera_USART_TX_GPIO_PORT;
	camera_uart_device.uart_tx_pin = Camera_USART_TX_GPIO_PIN;
	camera_uart_device.uart_gpio_RCC = Camera_USART_GPIO_CLK;
	camera_uart_device.uart_RCC = Camera_USART_CLK;
	camera_uart_device.uart_irq = Camera_USART_IRQ;
	camera_uart_device.baudrate = Camera_USART_BAUDRATE;
	camera_uart_device.dma_address = Camera_USART_DR_ADDRESS;
	camera_uart_device.dma_channel = Camera_USART_RX_DMA_CHANNEL;
	camera_uart_device.usart_Rx_8_buf = camera_Usart_Rx_Buf;   // ��8λ�޷��Ž���
	camera_uart_device.buff_size = Camera_USART_RBUFF_SIZE;
	camera_uart_device.uart_PrePriority = 1;
	camera_uart_device.uart_subPriority = 0;
	
	my_uart_DMA_Config(debug_uart_device);
	my_uart_DMA_Config(camera_uart_device);
	my_uart_Config(debug_uart_device);
	my_uart_Config(camera_uart_device);
}
// �����жϷ�����
void DEBUG_USART_IRQHandler(void)
{
	rt_interrupt_enter();
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_IDLE)!=RESET)
	{		
		//Uart_DMA_Rx_Data();       /* �ͷ�һ���ź�������ʾ�����ѽ��� */
		my_uart_DMA_Rx_Data(debug_uart_device, sem_debug_uart);
		USART_ReceiveData(DEBUG_USARTx); /* �����־λ */
	}	 
	rt_interrupt_leave();
}
void Camera_USART_IRQHandler(void)
{
	rt_interrupt_enter();
	if(USART_GetITStatus(Camera_USARTx,USART_IT_IDLE)!=RESET)
	{		
      my_uart_DMA_Rx_Data(camera_uart_device, sem_camera_uart);       /* �ͷ�һ���ź�������ʾ�����ѽ��� */
      USART_ReceiveData(Camera_USARTx); /* �����־λ */
	}	 
	rt_interrupt_leave();
}
 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */

static void uart_NVIC_Configuration(My_uart_device uart_device)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = uart_device.uart_irq;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = uart_device.uart_PrePriority;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = uart_device.uart_subPriority;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}
 /**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */


void my_uart_Config(My_uart_device uart_device)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��	
	RCC_APB2PeriphClockCmd(uart_device.uart_gpio_RCC, ENABLE);
	
	// �򿪴��������ʱ��
	if(uart_device.uart_RCC == RCC_APB2Periph_USART1)
		RCC_APB2PeriphClockCmd(uart_device.uart_RCC, ENABLE);
	else
		RCC_APB1PeriphClockCmd(uart_device.uart_RCC, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = uart_device.uart_tx_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(uart_device.uart_tx_port, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = uart_device.uart_rx_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(uart_device.uart_rx_port, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = uart_device.baudrate;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(uart_device.uart_module, &USART_InitStructure);
	// �����ж����ȼ�����
	uart_NVIC_Configuration(uart_device);
	// ���� ���ڿ���IDEL �ж�
	USART_ITConfig(uart_device.uart_module, USART_IT_IDLE, ENABLE);  
  // ��������DMA����
	USART_DMACmd(uart_device.uart_module, USART_DMAReq_Rx, ENABLE); 
	// ʹ�ܴ���
	USART_Cmd(uart_device.uart_module, ENABLE);	    
}


 /**
  * @brief  Uart��DMA����
  * @param  ��
  * @retval ��
  */

void my_uart_DMA_Config(My_uart_device uart_device)
{
		DMA_InitTypeDef DMA_InitStructure;
	
		// ����DMAʱ��
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);   // uart1~3����DMA1
		// ����DMAԴ��ַ���������ݼĴ�����ַ*/
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)uart_device.dma_address;
		// �ڴ��ַ(Ҫ����ı�����ָ��)
		if(uart_device.uart_module == Camera_USARTx)
			DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart_device.usart_Rx_8_buf;
		else
			DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart_device.usart_Rx_Buf;
		// ���򣺴����赽�ڴ�	
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		// �����С	
		DMA_InitStructure.DMA_BufferSize = uart_device.buff_size;
		// �����ַ����	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		// �ڴ��ַ����
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		// �������ݵ�λ���ֽ�
		DMA_InitStructure.DMA_PeripheralDataSize = 
	    DMA_PeripheralDataSize_Byte;
		// �ڴ����ݵ�λ���ֽ�
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
		// DMAģʽ��һ�λ���ѭ��ģʽ
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
		// ���ȼ�����	
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; 
		// ��ֹ�ڴ浽�ڴ�Ĵ���
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		// ����DMAͨ��		   
		DMA_Init(uart_device.dma_channel, &DMA_InitStructure);		
    // ���DMA���б�־
    DMA_ClearFlag(DMA1_FLAG_GL5);
    DMA_ITConfig(uart_device.dma_channel, DMA_IT_TE, ENABLE);
		// ʹ��DMA
		DMA_Cmd (uart_device.dma_channel,ENABLE);
}
 /**
  * @brief  Uart��DMA��������
  * @param  ��
  * @retval ��
  */

void my_uart_DMA_Rx_Data(My_uart_device uart_device, rt_sem_t sem_uart)
{
   // �ر�DMA ����ֹ����
   DMA_Cmd(uart_device.dma_channel, DISABLE);      
   // ��DMA��־λ
   DMA_ClearFlag( DMA1_FLAG_GL5 );          
   //  ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ
   uart_device.dma_channel->CNDTR = uart_device.buff_size;    
   DMA_Cmd(uart_device.dma_channel, ENABLE);       
   //������ֵ�ź��� �����ͽ��յ������ݱ�־����ǰ̨�����ѯ
   rt_sem_release(sem_uart);  
  /* 
    DMA �������ȴ����ݡ�ע�⣬����жϷ�������֡�����ʺܿ죬MCU����������˴ν��յ������ݣ�
    �ж��ַ������ݵĻ������ﲻ�ܿ������������ݻᱻ���ǡ���2�ַ�ʽ�����

    1. �����¿�������DMAͨ��֮ǰ����LumMod_Rx_Buf��������������ݸ��Ƶ�����һ�������У�
    Ȼ���ٿ���DMA��Ȼ�����ϴ����Ƴ��������ݡ�

    2. ����˫���壬��LumMod_Uart_DMA_Rx_Data�����У���������DMA_MemoryBaseAddr �Ļ�������ַ��
    ��ô�´ν��յ������ݾͻᱣ�浽�µĻ������У������ڱ����ǡ�
  */
}

/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* ����һ���ֽ����ݵ�USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	
	/* ���͸߰�λ */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}


