/*******************************************************************************
                              ���Ӻ궨��
*******************************************************************************/
// �����ﶨ���õ��ĸ�������
#define STM32f103ZET6_alien
//#define STM32f103VET6_small
//#define STM32f103C8T6_mostsmall

/*******************************************************************************
                              ���ڶ˿ں궨��
*******************************************************************************/
/** 
  * ����ͬ�Ĵ��ڹ��ص����ߺ�IO��һ������ֲʱ��Ҫ�޸��⼸����
	* 1-�޸�����ʱ�ӵĺ꣬uart1���ص�apb2���ߣ�����uart���ص�apb1����
	* 2-�޸�GPIO�ĺ�
  */
	
// ���ڶ�Ӧ��DMA����ͨ��
#define  USART_RX_DMA_CHANNEL     DMA1_Channel5
// ����Ĵ�����ַ
#define  USART_DR_ADDRESS        (&DEBUG_USARTx->DR)
// һ�η��͵�������
#define  USART_RBUFF_SIZE            1000 

// ����1-USART1
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO ���ź궨��
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler
/*******************************************************************************
                              AT24C02�˿ڶ���
*******************************************************************************/

#if defined(STM32f103VET6_small)
/*  AT24cxx_IIC_SCLʱ�Ӷ˿ڡ����Ŷ��� */
#define AT24cxx_IIC_SCL_PORT 			GPIOE   
#define AT24cxx_IIC_SCL_PIN 			(GPIO_Pin_11)
#define AT24cxx_IIC_SCL_PORT_RCC		RCC_APB2Periph_GPIOE

/*  AT24cxx_IIC_SDAʱ�Ӷ˿ڡ����Ŷ��� */
#define AT24cxx_IIC_SDA_PORT 			GPIOE  
#define AT24cxx_IIC_SDA_PIN 			(GPIO_Pin_12)
#define AT24cxx_IIC_SDA_PORT_RCC		RCC_APB2Periph_GPIOE

//IO��������	 
#define AT24cxx_IIC_SCL    PEout(11) //SCL
#define AT24cxx_IIC_SDA    PEout(12) //SDA	 
#define AT24cxx_READ_SDA   PEin(12)  //����SDA

#endif
#if defined(STM32f103ZET6_alien)
/*  AT24cxx_IIC_SCLʱ�Ӷ˿ڡ����Ŷ��� */
#define AT24cxx_IIC_SCL_PORT 			GPIOE   
#define AT24cxx_IIC_SCL_PIN 			(GPIO_Pin_11)
#define AT24cxx_IIC_SCL_PORT_RCC		RCC_APB2Periph_GPIOE

/*  AT24cxx_IIC_SDAʱ�Ӷ˿ڡ����Ŷ��� */
#define AT24cxx_IIC_SDA_PORT 			GPIOE  
#define AT24cxx_IIC_SDA_PIN 			(GPIO_Pin_12)
#define AT24cxx_IIC_SDA_PORT_RCC		RCC_APB2Periph_GPIOE

//IO��������	 
#define AT24cxx_IIC_SCL    PEout(11) //SCL
#define AT24cxx_IIC_SDA    PEout(12) //SDA	 
#define AT24cxx_READ_SDA   PEin(12)  //����SDA
#endif
/*******************************************************************************
                              �����˿ں궨��
*******************************************************************************/
#if defined(STM32f103VET6_small) 
#define KEY1_Pin    GPIO_Pin_0    //����K_1�ܽ�
#define KEY2_Pin    GPIO_Pin_1    //����K_2�ܽ�
#define KEY3_Pin    GPIO_Pin_2   //����K_3�ܽ�
#define KEY4_Pin    GPIO_Pin_3  //����K_4�ܽ�
#define KEY1_Port (GPIOA) //����˿�
#define KEY2_Port (GPIOA) //����˿�
#define KEY3_Port (GPIOA) //����˿�
#define KEY4_Port (GPIOA) //����˿�
//ʹ��λ��������
#define K_1 PAin(0)
#define K_2 PAin(1)
#define K_3 PAin(2)
#define K_4 PAin(3)
#endif
#if defined(STM32f103ZET6_alien) 
#define KEY1_Pin    GPIO_Pin_4    //����K_1�ܽ�
#define KEY2_Pin    GPIO_Pin_5    //����K_2�ܽ�
#define KEY3_Pin    GPIO_Pin_6   //����K_3�ܽ�
#define KEY4_Pin    GPIO_Pin_7  //����K_4�ܽ�
#define KEY1_Port (GPIOA) //����˿�
#define KEY2_Port (GPIOA) //����˿�
#define KEY3_Port (GPIOA) //����˿�
#define KEY4_Port (GPIOA) //����˿�
//ʹ��λ��������
#define K_1 PAin(4)
#define K_2 PAin(5)
#define K_3 PAin(6)
#define K_4 PAin(7)
#endif
/*******************************************************************************
                              OLED�˿ں궨��
*******************************************************************************/
//-----------------OLED�˿ڶ���----------------  
#if defined(STM32f103VET6_small)
#define OLED_CS_PORT  GPIOD
#define OLED_RCC_CS_PORT  RCC_APB2Periph_GPIOD
#define OLED_CS_PIN   GPIO_Pin_3

#define OLED_RST_PORT  GPIOD
#define OLED_RCC_RST_PORT  RCC_APB2Periph_GPIOD
#define OLED_RST_PIN  GPIO_Pin_4

#define OLED_DC_PORT  GPIOD
#define OLED_RCC_DC_PORT  RCC_APB2Periph_GPIOD
#define OLED_DC_PIN   GPIO_Pin_5

#define OLED_SCLK_PORT  GPIOD
#define OLED_RCC_SCLK_PORT  RCC_APB2Periph_GPIOD
#define OLED_SCLK_PIN  GPIO_Pin_7

#define OLED_SDIN_PORT  GPIOD
#define OLED_RCC_SDIN_PORT  RCC_APB2Periph_GPIOD
#define OLED_SDIN_PIN  GPIO_Pin_8
#endif
#if defined(STM32f103ZET6_alien)
#define OLED_CS_PORT  GPIOC
#define OLED_RCC_CS_PORT  RCC_APB2Periph_GPIOC
#define OLED_CS_PIN   GPIO_Pin_13

#define OLED_RST_PORT  GPIOE
#define OLED_RCC_RST_PORT  RCC_APB2Periph_GPIOE
#define OLED_RST_PIN  GPIO_Pin_3

#define OLED_DC_PORT  GPIOE
#define OLED_RCC_DC_PORT  RCC_APB2Periph_GPIOE
#define OLED_DC_PIN   GPIO_Pin_5            

#define OLED_SCLK_PORT  GPIOB                              //SCLK(D0)
#define OLED_RCC_SCLK_PORT  RCC_APB2Periph_GPIOB
#define OLED_SCLK_PIN  GPIO_Pin_9

#define OLED_SDIN_PORT  GPIOE                             //SDIN(D1)
#define OLED_RCC_SDIN_PORT  RCC_APB2Periph_GPIOE
#define OLED_SDIN_PIN  GPIO_Pin_1
#endif