/*******************************************************************************
                              板子宏定义
*******************************************************************************/
// 在这里定义用的哪个开发板
#define STM32f103ZET6_alien
//#define STM32f103VET6_small
//#define STM32f103C8T6_mostsmall

/*******************************************************************************
                              串口端口宏定义
*******************************************************************************/
/** 
  * ，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
	* 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
	* 2-修改GPIO的宏
  */
	
// 串口对应的DMA请求通道
#define  USART_RX_DMA_CHANNEL     DMA1_Channel5
// 外设寄存器地址
#define  USART_DR_ADDRESS        (&DEBUG_USARTx->DR)
// 一次发送的数据量
#define  USART_RBUFF_SIZE            1000 

// 串口1-USART1
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler
/*******************************************************************************
                              AT24C02端口定义
*******************************************************************************/

#if defined(STM32f103VET6_small)
/*  AT24cxx_IIC_SCL时钟端口、引脚定义 */
#define AT24cxx_IIC_SCL_PORT 			GPIOE   
#define AT24cxx_IIC_SCL_PIN 			(GPIO_Pin_11)
#define AT24cxx_IIC_SCL_PORT_RCC		RCC_APB2Periph_GPIOE

/*  AT24cxx_IIC_SDA时钟端口、引脚定义 */
#define AT24cxx_IIC_SDA_PORT 			GPIOE  
#define AT24cxx_IIC_SDA_PIN 			(GPIO_Pin_12)
#define AT24cxx_IIC_SDA_PORT_RCC		RCC_APB2Periph_GPIOE

//IO操作函数	 
#define AT24cxx_IIC_SCL    PEout(11) //SCL
#define AT24cxx_IIC_SDA    PEout(12) //SDA	 
#define AT24cxx_READ_SDA   PEin(12)  //输入SDA

#endif
#if defined(STM32f103ZET6_alien)
/*  AT24cxx_IIC_SCL时钟端口、引脚定义 */
#define AT24cxx_IIC_SCL_PORT 			GPIOE   
#define AT24cxx_IIC_SCL_PIN 			(GPIO_Pin_11)
#define AT24cxx_IIC_SCL_PORT_RCC		RCC_APB2Periph_GPIOE

/*  AT24cxx_IIC_SDA时钟端口、引脚定义 */
#define AT24cxx_IIC_SDA_PORT 			GPIOE  
#define AT24cxx_IIC_SDA_PIN 			(GPIO_Pin_12)
#define AT24cxx_IIC_SDA_PORT_RCC		RCC_APB2Periph_GPIOE

//IO操作函数	 
#define AT24cxx_IIC_SCL    PEout(11) //SCL
#define AT24cxx_IIC_SDA    PEout(12) //SDA	 
#define AT24cxx_READ_SDA   PEin(12)  //输入SDA
#endif
/*******************************************************************************
                              按键端口宏定义
*******************************************************************************/
#if defined(STM32f103VET6_small) 
#define KEY1_Pin    GPIO_Pin_0    //定义K_1管脚
#define KEY2_Pin    GPIO_Pin_1    //定义K_2管脚
#define KEY3_Pin    GPIO_Pin_2   //定义K_3管脚
#define KEY4_Pin    GPIO_Pin_3  //定义K_4管脚
#define KEY1_Port (GPIOA) //定义端口
#define KEY2_Port (GPIOA) //定义端口
#define KEY3_Port (GPIOA) //定义端口
#define KEY4_Port (GPIOA) //定义端口
//使用位操作定义
#define K_1 PAin(0)
#define K_2 PAin(1)
#define K_3 PAin(2)
#define K_4 PAin(3)
#endif
#if defined(STM32f103ZET6_alien) 
#define KEY1_Pin    GPIO_Pin_4    //定义K_1管脚
#define KEY2_Pin    GPIO_Pin_5    //定义K_2管脚
#define KEY3_Pin    GPIO_Pin_6   //定义K_3管脚
#define KEY4_Pin    GPIO_Pin_7  //定义K_4管脚
#define KEY1_Port (GPIOA) //定义端口
#define KEY2_Port (GPIOA) //定义端口
#define KEY3_Port (GPIOA) //定义端口
#define KEY4_Port (GPIOA) //定义端口
//使用位操作定义
#define K_1 PAin(4)
#define K_2 PAin(5)
#define K_3 PAin(6)
#define K_4 PAin(7)
#endif
/*******************************************************************************
                              OLED端口宏定义
*******************************************************************************/
//-----------------OLED端口定义----------------  
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