#include "bsp_usart.h"
#include "rtthread.h"

/* 外部定义信号量控制块 */
extern rt_sem_t sem_debug_uart;
extern rt_sem_t sem_camera_uart;
char debug_Usart_Rx_Buf[DEBUG_USART_RBUFF_SIZE];  // 定义存储空间
uint8_t camera_Usart_Rx_Buf[Camera_USART_RBUFF_SIZE];  // 这个空间要在外面定义，直接在结构体中定义这个数组，dma就传不进去，不知为啥
My_uart_device debug_uart_device, camera_uart_device;  // 全局变量
void my_uart_DMA_Rx_Data(My_uart_device uart_device, rt_sem_t sem_uart);
void my_uart_DMA_Config(My_uart_device uart_device);
void my_uart_Config(My_uart_device uart_device);

 /**
  * @brief  配置所有串口设备
  * @param  无
  * @retval 无
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
	debug_uart_device.uart_PrePriority = 1; // 抢占优先级
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
	camera_uart_device.usart_Rx_8_buf = camera_Usart_Rx_Buf;   // 用8位无符号接受
	camera_uart_device.buff_size = Camera_USART_RBUFF_SIZE;
	camera_uart_device.uart_PrePriority = 1;
	camera_uart_device.uart_subPriority = 0;
	
	my_uart_DMA_Config(debug_uart_device);
	my_uart_DMA_Config(camera_uart_device);
	my_uart_Config(debug_uart_device);
	my_uart_Config(camera_uart_device);
}
// 串口中断服务函数
void DEBUG_USART_IRQHandler(void)
{
	rt_interrupt_enter();
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_IDLE)!=RESET)
	{		
		//Uart_DMA_Rx_Data();       /* 释放一个信号量，表示数据已接收 */
		my_uart_DMA_Rx_Data(debug_uart_device, sem_debug_uart);
		USART_ReceiveData(DEBUG_USARTx); /* 清除标志位 */
	}	 
	rt_interrupt_leave();
}
void Camera_USART_IRQHandler(void)
{
	rt_interrupt_enter();
	if(USART_GetITStatus(Camera_USARTx,USART_IT_IDLE)!=RESET)
	{		
      my_uart_DMA_Rx_Data(camera_uart_device, sem_camera_uart);       /* 释放一个信号量，表示数据已接收 */
      USART_ReceiveData(Camera_USARTx); /* 清除标志位 */
	}	 
	rt_interrupt_leave();
}
 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */

static void uart_NVIC_Configuration(My_uart_device uart_device)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = uart_device.uart_irq;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = uart_device.uart_PrePriority;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = uart_device.uart_subPriority;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}
 /**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */


void my_uart_Config(My_uart_device uart_device)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟	
	RCC_APB2PeriphClockCmd(uart_device.uart_gpio_RCC, ENABLE);
	
	// 打开串口外设的时钟
	if(uart_device.uart_RCC == RCC_APB2Periph_USART1)
		RCC_APB2PeriphClockCmd(uart_device.uart_RCC, ENABLE);
	else
		RCC_APB1PeriphClockCmd(uart_device.uart_RCC, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = uart_device.uart_tx_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(uart_device.uart_tx_port, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = uart_device.uart_rx_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(uart_device.uart_rx_port, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = uart_device.baudrate;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(uart_device.uart_module, &USART_InitStructure);
	// 串口中断优先级配置
	uart_NVIC_Configuration(uart_device);
	// 开启 串口空闲IDEL 中断
	USART_ITConfig(uart_device.uart_module, USART_IT_IDLE, ENABLE);  
  // 开启串口DMA接收
	USART_DMACmd(uart_device.uart_module, USART_DMAReq_Rx, ENABLE); 
	// 使能串口
	USART_Cmd(uart_device.uart_module, ENABLE);	    
}


 /**
  * @brief  Uart的DMA配置
  * @param  无
  * @retval 无
  */

void my_uart_DMA_Config(My_uart_device uart_device)
{
		DMA_InitTypeDef DMA_InitStructure;
	
		// 开启DMA时钟
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);   // uart1~3都是DMA1
		// 设置DMA源地址：串口数据寄存器地址*/
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)uart_device.dma_address;
		// 内存地址(要传输的变量的指针)
		if(uart_device.uart_module == Camera_USARTx)
			DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart_device.usart_Rx_8_buf;
		else
			DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)uart_device.usart_Rx_Buf;
		// 方向：从外设到内存	
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		// 传输大小	
		DMA_InitStructure.DMA_BufferSize = uart_device.buff_size;
		// 外设地址不增	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		// 内存地址自增
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		// 外设数据单位是字节
		DMA_InitStructure.DMA_PeripheralDataSize = 
	    DMA_PeripheralDataSize_Byte;
		// 内存数据单位是字节
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
		// DMA模式，一次或者循环模式
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
		// 优先级：中	
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; 
		// 禁止内存到内存的传输
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		// 配置DMA通道		   
		DMA_Init(uart_device.dma_channel, &DMA_InitStructure);		
    // 清除DMA所有标志
    DMA_ClearFlag(DMA1_FLAG_GL5);
    DMA_ITConfig(uart_device.dma_channel, DMA_IT_TE, ENABLE);
		// 使能DMA
		DMA_Cmd (uart_device.dma_channel,ENABLE);
}
 /**
  * @brief  Uart的DMA接收数据
  * @param  无
  * @retval 无
  */

void my_uart_DMA_Rx_Data(My_uart_device uart_device, rt_sem_t sem_uart)
{
   // 关闭DMA ，防止干扰
   DMA_Cmd(uart_device.dma_channel, DISABLE);      
   // 清DMA标志位
   DMA_ClearFlag( DMA1_FLAG_GL5 );          
   //  重新赋值计数值，必须大于等于最大可能接收到的数据帧数目
   uart_device.dma_channel->CNDTR = uart_device.buff_size;    
   DMA_Cmd(uart_device.dma_channel, ENABLE);       
   //给出二值信号量 ，发送接收到新数据标志，供前台程序查询
   rt_sem_release(sem_uart);  
  /* 
    DMA 开启，等待数据。注意，如果中断发送数据帧的速率很快，MCU来不及处理此次接收到的数据，
    中断又发来数据的话，这里不能开启，否则数据会被覆盖。有2种方式解决：

    1. 在重新开启接收DMA通道之前，将LumMod_Rx_Buf缓冲区里面的数据复制到另外一个数组中，
    然后再开启DMA，然后马上处理复制出来的数据。

    2. 建立双缓冲，在LumMod_Uart_DMA_Rx_Data函数中，重新配置DMA_MemoryBaseAddr 的缓冲区地址，
    那么下次接收到的数据就会保存到新的缓冲区中，不至于被覆盖。
  */
}

/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}


